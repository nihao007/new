/**
 * 3 板共线总线关节伺服 —— 控制核心参考 user(PID完成2) 级联 PID 实现
 * 控制律 (简单级联, 无状态机):
 *   位置环 40ms: ADC 三点标定 → 动态积分限幅 (lim=|pe|*100+500) → BldcPos_Exec 算速度目标
 *   速度环 5ms:  BldcSpd_Run (cmdRpm 斜坡 + actRpm 斜坡 + 速度 PI) → duty/FR
 *   新目标: Bldc_SetTargetDeg 预置积分 holdIntegral + 放宽积分限幅, 位置环 40ms 后收窄
 *   死区保持: |pe|<2° → 小 P (holdKp) 微速, |cmdRpm|<1 → duty=积分 (保持力)
 * 总线 (UART1, 115200, 多板共线, 跳线 GPIO2_7/GPIO3_0 识别本板):
 *   帧: [AA][ID][CMD][目标/填充][SUM]  SUM=除校验字节外累加 & 0xFF (统一 5B, 无父角)
 *   ID=电机号 01~04, 跳线决定本板响应哪些: 00→04(无刷单板) / 01→02+03(双无刷) / 10→01(步进)
 *   读: [AA][ID][01][X][SUM] → 回 [AA][ID][世界角][SUM] (X 忽略)
 *   写: [AA][ID][00][目标][SUM] → 执行, 不应答 (目标: M2/M3 世界角, M1/M4 局部)
 *   急停: [AA][ID][02][X][SUM] (ID 任意) → 本板所有电机停止, 不应答
 *   M3 世界角 = M2 ADC 实测 + M3 局部 − 90 (世界角闭环, 位置环每拍用 M2)
 * 遥测: DIAG_EN=1 时每 500ms 打印本板电机 cur/tgt/duty/cmd/rpm/en
 */

#include "typedefs.h"
#include <math.h>
#include "feature.h"
#include "main.h"
#include "gpio.h"
#include "clock.h"
#include "debug.h"
#include "adc/adc_user.h"
#include "uart/uart_ring.h"
#include "uart/uart_bus.h"
#include "motors/motor_bldc.h"
#include "motors/motor_stepper.h"

/* ================================================================
 *  硬件句柄 (全局定义, 给 system_init 和各模块使用)
 * ================================================================ */
GPT_Handle g_gptHandle0;    /* 电机2 PWM */
GPT_Handle g_gptHandle1;    /* 电机3/电机4 PWM */
UART_Handle g_uart1;
ADC_Handle g_adc0;          /* 电机1 (步进) 电位器: ADC0 SOC0=A5 (GPIO3_2) */
ADC_Handle g_adc1;          /* 诊断: ADC1 SOC0=A2 (GPIO2_0) */
ADC_Handle g_adc2;          /* 电机3 角度: ADC2 SOC0=A2 (GPIO6_0) */
ADC_Handle g_adc3;          /* 电机2 电位器: ADC3 SOC0=A15 (GPIO16_5) — 实际接线 */
APT_Handle g_aptStepper;    /* 电机1 步进: APT0 硬件脉冲 (GPIO3_4) */

GPIO_Handle g_gpio2_2, g_gpio4_6, g_gpio4_7, g_gpio5_3, g_gpio5_4;
GPIO_Handle g_gpio6_2, g_gpio6_7;
/* 电机1 步进 (I2C0 四轴版 IO): DIR=GPIO7_5 REF=GPIO7_6 MS3=GPIO3_5
 * SLEEP=GPIO4_0 RESET=GPIO4_1 MS2=GPIO5_6 MS1=GPIO5_7 EN=GPIO6_5 */
GPIO_Handle g_gpio7_5, g_gpio7_6, g_gpio6_5, g_gpio4_0, g_gpio4_1;
GPIO_Handle g_gpio5_6, g_gpio5_7, g_gpio3_5;
TIMER_Handle g_timerCtrl, g_timerFg;

/* ================================================================
 *  无刷伺服实例 (控制参数抄 user(PID完成2) 的 g_motor2/3, 硬件映射按本工程)
 *  电机2: spd kp=0.5/ki=0.2 gravBase=5657(重力补偿) holdIntegral=5000 (userPID完成 原样)
 *  电机3: 同上 holdIntegral=1000, pos kp=30 (肘, 负载重)
 *  电机4: 04 单关节板, 抄电机3 参数 (aligned=0 独立关节, 标定待实测)
 *  FB: timerHz=200MHz (BASE_FUNC_GetTick), firstEdge=true 首沿作参考
 * ================================================================ */
Motor_Bldc g_motor[3] = {
    /* 电机2 (肩, 双无刷板) */
    {
        .id = 0x02,
        .pwm = &g_gptHandle0,
        .frGpio = &g_gpio2_2, .frPin = GPIO_PIN_2,
        .bkGpio = &g_gpio4_6, .bkPin = GPIO_PIN_6,
        .adc = &g_adc3,          /* 电机2 电位器: ADC3 (GPIO16_5) */
        .fb = {.timerHz = 200000000UL, .fc = 5.0f, .firstEdge = true},
        .spd = {.pid = PIDCTRL_INIT(0.5f, 0.2f, 0, 0, DUTY_MAX, -5000, 5000),
                .gravBase = 5657, .rampRate = 1000, .cmdRate = 800,
                .holdIntegral = 5000},
        .pos = {.pid = PIDCTRL_INIT(5.0f, 0, 0, 0, 1000, 0, 1000),
                .deadband = 2.0f, .holdKp = 30, .maxSpeed = 1000, .decelRate = 1000,
                .ffUp = 2000, .ffDown = 1000},
    },
    /* 电机3 (肘, 双无刷板) */
    {
        .id = 0x03,
        .pwm = &g_gptHandle1,
        .frGpio = &g_gpio4_7, .frPin = GPIO_PIN_7,
        .bkGpio = &g_gpio6_7, .bkPin = GPIO_PIN_7,
        .adc = &g_adc2,
        .fb = {.timerHz = 200000000UL, .fc = 5.0f, .firstEdge = true},
        .spd = {.pid = PIDCTRL_INIT(12.0f, 2.0f, 0, 0, DUTY_MAX, -5000, 5000),
                .gravBase = 5500, .startupDuty = 5700, .rampRate = 1000, .cmdRate = 800,   /* kp12: 净推力 352→528 (78° 卡住推力小) */
                .holdIntegral = 1000},
        .pos = {.pid = PIDCTRL_INIT(5.0f, 0, 0, 0, 1000, 0, 1000),
                .deadband = 6.0f, .holdKp = 5, .maxSpeed = 300, .decelRate = 500,   /* maxSpeed 300: 位置环速度恢复 (80 太慢爬不动) */
                .ffUp = 6000, .ffDown = 3000},
    },
    /* 电机4 (04 单关节板 跳线00) — 与电机2 同引脚 (GPT0 PWM / GPIO2_2 FR / GPIO4_6 BK) */
    {
        .id = 0x04,
        .pwm = &g_gptHandle0,
        .frGpio = &g_gpio2_2, .frPin = GPIO_PIN_2,
        .bkGpio = &g_gpio4_6, .bkPin = GPIO_PIN_6,
        .adc = &g_adc3,          /* 04 板电位器 (板载 ADC0 有问题 → 实际走 ADC3) */
        .fb = {.timerHz = 200000000UL, .fc = 5.0f, .firstEdge = true},
        .spd = {.pid = PIDCTRL_INIT(3.0f, 1.0f, 0, 0, DUTY_MAX, -5000, 5000),
                .gravBase = 500, .rampRate = 300, .cmdRate = 300,   /* M4 末端关节轻负载: gravBase 500 (抖动大→调小), 待实测校准 */
                .holdIntegral = 1000},
        .pos = {.pid = PIDCTRL_INIT(30.0f, 0, 0, 0, 1000, 0, 1000),
                .deadband = 2.0f, .holdKp = 30, .maxSpeed = 300, .decelRate = 1000,   /* maxSpeed 300: 位置环限速 */
                .ffUp = 6000, .ffDown = 3000},
    },
};

/* 顺齐偏移: 电机2=0, 电机3=90 (相对电机2 管道), 电机4=0 (独立关节) */
static const float g_aligned[3] = {0.0f, 90.0f, 0.0f};

/* M4 父臂 = M3 世界角: 04 单关节板上无 M3 电位器实测,
 * 写死为 M3 目标角 90° — 根据需要修改 */
#define M4_M3_WORLD_DEG 90.0f

/* 电位器三点标定 (ADC→角度, 支持 ADC 正/反向, 排序后分段线性):
 * 电机2 机械限位 23~150° (倾角仪实测), 电机3/4 局部标度 0~180° */
typedef struct {
    uint16_t adc0, adc90, adc180;
    float deg0, deg90, deg180;
} AngleCal;

static const AngleCal g_cal[3] = {
    {774, 1151, 1958, 23.0f, 90.0f, 150.0f},   /* 电机2 (肩) */
    {1471, 738, 484, 0.0f, 90.0f, 180.0f},     /* 电机3 (肘, ADC 反向) */
    {1471, 738, 484, 0.0f, 90.0f, 180.0f},     /* 电机4 (单关节, TODO 待实测标定) */
};

/* ================================================================
 *  电机1: 步进 (ID01, I2C0 四轴版 IO: APT0 硬件脉冲, 非阻塞)
 *  STEP=GPIO3_4(APT0_PWMA) DIR=GPIO7_5 EN=GPIO6_5 SLEEP=GPIO4_0 RESET=GPIO4_1
 *  MS1=GPIO5_7 MS2=GPIO5_6 MS3=GPIO3_5 REF=GPIO7_6 电位器=ADC0 (GPIO3_2)
 * ================================================================ */
Motor_Stepper g_motor1 = {
    .id = 0x01,
    .apt = &g_aptStepper,       /* APT0 硬件脉冲 (GPIO3_4), 唯一模式 */
    .dirGpio = &g_gpio7_5,
    .dirPin = GPIO_PIN_5,
    .enGpio = &g_gpio6_5,
    .enPin = GPIO_PIN_5,
    .sleepGpio = &g_gpio4_0,
    .sleepPin = GPIO_PIN_0,
    .resetGpio = &g_gpio4_1,
    .resetPin = GPIO_PIN_1,
    .ms1Gpio = &g_gpio5_7,
    .ms1Pin = GPIO_PIN_7,
    .ms2Gpio = &g_gpio5_6,
    .ms2Pin = GPIO_PIN_6,
    .ms3Gpio = &g_gpio3_5,
    .ms3Pin = GPIO_PIN_5,
    .refGpio = &g_gpio7_6,
    .refPin = GPIO_PIN_6,
    .adc = &g_adc0,             /* 电位器 ADC0 (GPIO3_2) */
    .adc0deg = 3000,
    .adc180deg = 600,
    .degPerStep = 0.197f,
};

/* ================================================================
 *  总线协议 (多板共线, 跳线 GPIO2_7/GPIO3_0 识别本板)
 *  帧: [AA][ID][CMD][父臂][数据][SUM]  SUM=除校验字节外累加 & 0xFF
 *  读: [AA][ID][01][父臂][SUM] (5字节) → 回 [AA][ID][世界角][SUM]
 *  写: [AA][ID][00][父臂][世界目标][SUM] (6字节) → 执行, 不应答
 *  急停: [AA][ID][02][SUM] (5字节, ID 任意=广播) → 本板所有电机停止, 不应答
 *  ID=电机号 01~04, 跳线决定本板响应哪些电机:
 *    跳线 00 → 电机04 (无刷单板) / 跳线 01 → 电机02+03 (双无刷板) / 跳线 10 → 电机01 (步进板)
 *  世界角 = 父臂角 + 局部角 − 顺齐偏移; 双无刷板电机3 的父臂 = 板内实测电机2 角 (ADC3), 帧父臂忽略
 * ================================================================ */
#define FRAME_HEADER 0xAA

static uint8_t g_boardId = 0;       /* 本板跳线地址 */
static uint8_t g_frame[5];          /* 帧缓冲区 (统一 5 字节, 无父角) */
static volatile uint8_t g_frameLen = 0;

/* 各电机电位器 ADC 读数 (跳线分板, 只对应板有效) */
static uint16_t adc1 = 0;           /* 电机1 (步进) 电位器读数 */

/* 当前局部角缓存 (位置环 40ms 刷新): 读命令/遥测/父臂共用 */
static float g_degS[3];
static uint16_t g_adcRaw[3];   /* 位置环原始 ADC 读数 (诊断: 恒定=通道错/悬空) */

/* 诊断日志开关: 1=开 (单板调试用, 帧/执行打印), 0=关 (多板共线部署, TX 不占总线) */
#define DIAG_EN 1

/* 三点标定查表: ADC→角度 (排序后分段线性, 支持 ADC 正/反向) */
static float AngleToDeg(const AngleCal *c, uint16_t val)
{
    uint16_t a[3] = {c->adc0, c->adc90, c->adc180};
    float d[3] = {c->deg0, c->deg90, c->deg180};
    int i, j;

    for (i = 1; i < 3; i++) {               /* 按 adc 升序插入排序 (成对交换) */
        uint16_t ta = a[i];
        float td = d[i];
        for (j = i; j > 0 && a[j - 1] > ta; j--) {
            a[j] = a[j - 1];
            d[j] = d[j - 1];
        }
        a[j] = ta;
        d[j] = td;
    }
    if (val <= a[0])
        return d[0];
    for (i = 1; i < 3; i++) {
        if (val <= a[i]) {
            if (a[i] == a[i - 1])
                return d[i];
            return d[i - 1] + (float)(val - a[i - 1]) * (d[i] - d[i - 1])
                   / (float)(a[i] - a[i - 1]);
        }
    }
    return d[2];
}

/* 世界角 (读应答): g_degS 已按各电机存世界角 —
 * M2/M4 aligned=0 (世界=局部), M3 位置环算好 θ3w=θ2+θ3−90 */
static float Bus_WorldAngle(uint8_t id, float parentWorld)
{
    (void)parentWorld;
    return g_degS[id - 2];
}

/* 写伺服指令: 帧 data 即目标 (无父角 — 父臂/顺齐偏移全走 ADC 实测/世界角闭环);
 * Bldc_SetTargetDeg 预置积分 holdIntegral + 放宽积分限幅 */
static void Bus_ServoWrite(uint8_t id, uint8_t data)
{
    Motor_Bldc *m = &g_motor[id - 2];
    float local = (float)data;   /* M2/M3 世界目标; M4 独立关节目标 */

    if (local < 0.0f) local = 0.0f;
    if (local > 180.0f) local = 180.0f;
#if (DIAG_EN == 1)
    DBG_PRINTF("m%u cur=%.1f tgt=%.1f\r\n",
               (unsigned int)id, g_degS[id - 2], local);
#endif
    Bldc_SetTargetDeg(m, local);
}

/* 应答 [AA][ID][世界角][SUM]: 临时复用 TX 引脚发送, 发完切回 GPIO */
static void Bus_Reply(uint8_t id, float deg)
{
    unsigned char rep[4];
    if (deg < 0.0f)
        deg = 0.0f;
    rep[0] = FRAME_HEADER;
    rep[1] = id;
    rep[2] = (unsigned char)(deg + 0.5f);   /* 四舍五入 */
    if (rep[2] > 180)
        rep[2] = 180;
    rep[3] = (unsigned char)(rep[0] + rep[1] + rep[2]);   /* SUM */
    UartBus_TxMuxToUart();
    HAL_UART_WriteBlocking(&g_uart1, rep, 4, 10000);   /* blockingTime 超时 */
    UartBus_TxMuxToGpio();
}

/* 跳线 → 本板电机 ID 集合 (多板共线, 各板只响应自己的电机):
 *  跳线 00 → 电机04 (无刷单板) / 跳线 01 → 电机02+03 (双无刷板) / 跳线 10 → 电机01 (步进板)
 *  (bit0=GPIO3_0, bit1=GPIO2_7, 见 uart_bus.c UartBus_GetAddr) */
static int Bus_IsMyMotor(uint8_t id)
{
    switch (g_boardId) {                 /* 跳线分发 */
    case 2:  return (id == 0x01);
    case 1:  return (id == 0x02 || id == 0x03);
    case 0:  return (id == 0x04);
    default: return 0;
    }
}

/* 电机01 步进角度: ADC0 电位器 → 局部角 (0~180, 与 Stepper_RunToAngle 同一换算) */
static float Motor1_Angle(void)
{
    uint16_t val;
    float deg;

    ADC_Read(&g_adc0, &val);
    adc1 = val;                     /* 电机1 电位器读数 */
    deg = (float)((int)val - (int)g_motor1.adc0deg) * 180.0f
          / (float)((int)g_motor1.adc180deg - (int)g_motor1.adc0deg);
    if (deg < 0.0f) deg = 0.0f;
    if (deg > 180.0f) deg = 180.0f;
    return deg;
}

/* ================================================================
 *  中断回调
 * ================================================================ */

/* FG 分发 (跳线分板): 双无刷板(01) GPIO5_3=M2 FG, GPIO6_2=M3 FG; 04板(00) GPIO5_3=M4 FG */
void GPIO5_3_IrqCallback(void *p)
{
    (void)p;
    if (Bus_IsMyMotor(0x02))
        BldcFb_Isr(&g_motor[0], BASE_FUNC_GetTick());   /* 双无刷板: 电机2 FG */
    else if (Bus_IsMyMotor(0x04))
        BldcFb_Isr(&g_motor[2], BASE_FUNC_GetTick());   /* 04板: 电机4 FG */
}

void GPIO6_2_IrqCallback(void *p)
{
    (void)p;
    if (Bus_IsMyMotor(0x03))
        BldcFb_Isr(&g_motor[1], BASE_FUNC_GetTick());   /* 双无刷板: 电机3 FG */
}

static volatile uint8_t g_spdFlag = 0;  /* 5ms 速度环触发 (主循环消费) */
static volatile uint8_t g_posFlag = 0;  /* 40ms 位置环触发 (8×5ms) */
static uint8_t g_posCnt = 0;            /* 5ms→40ms 计数 */
static volatile uint8_t g_telFlag = 0;  /* 500ms 遥测触发 */
static uint16_t g_telCnt = 0;           /* 5ms→500ms 计数 */
static uint16_t g_posRun = 0;           /* 位置环运行计数 (诊断: 恒定=没跑) */
static uint16_t g_stallCnt[3] = {0};    /* 堵转计时 (每台电机) */
static uint16_t g_noFg[3] = {0};        /* 无 FG 计时: 200ms 无 FG → 转速归零 */

/* 5ms 控制时基 (TIMER1): ISR 只置标志 —— 环重活全在主循环 (ISR 要轻) */
void SpeedLoop_ISR(void *p)
{
    (void)p;
    g_spdFlag = 1;
    g_posCnt++;
    if (g_posCnt >= 4) {            /* 4×5ms=20ms 位置环 (用户要求) */
        g_posCnt = 0;
        g_posFlag = 1;
    }
#if (DIAG_EN == 1)
    g_telCnt++;
    if (g_telCnt >= 40) {           /* 40×5ms=200ms ADC 诊断 */
        g_telCnt = 0;
        g_telFlag = 1;
    }
#endif
}

/* 单台电机位置环: 动态积分限幅 → 位置 P → 速度目标 (idx=0/1/2 → 电机2/3/4) */
static void PosLoopOne(Motor_Bldc *m, int idx)
{
    float pe = m->pos.target - g_degS[idx];
    float lim = fabsf(pe) * 100.0f + 500.0f;   /* 动态积分限幅 */
    PIDCtrl_SetIntegralLimits(&m->spd.pid, -lim, lim);
    uint8_t dir;
    float spd = BldcPos_Exec(m, g_degS[idx], m->fb.updated, m->spd.enabled, &dir);
    if (fabsf(spd) > 0.5f) {
        m->spd.enabled = true;
        m->spd.dir = dir;
    }
    BldcSpd_SetTarget(m, spd);
}

/* 串口字节泵: 只收总线帧 (0xAA 包头), 非帧首字节丢弃等同步
 * 帧处理顺序: ① 收齐+校验 → ② 急停(广播) → ③ 跳线识别本板电机 → ④ 读写位
 * 读: 只上报角度, 不需要 PID, 报完等待下一帧
 * 写: 局部目标 = 世界 − 父臂 + 顺齐偏移, 转执行
 * 非本板电机 → 只收不应答 (TX 保持 GPIO 高阻) */
static void UartPump(void)
{
    while (UART_Ring_HasData()) {
        uint8_t b = UART_Ring_ReadByte();
        if (g_frameLen == 0 && b != FRAME_HEADER)
            continue;                 /* 非帧首字节丢弃, 等包头 0xAA 同步 */
        if (g_frameLen < sizeof(g_frame))
            g_frame[g_frameLen++] = b;
        if (g_frameLen >= 2 && g_frame[1] > 4) {
            g_frameLen = 0;           /* ID 非法 (0=广播急停, 1~4=电机号), 重新同步 */
            continue;
        }
        if (g_frameLen < 3)
            continue;                 /* 还没到 CMD 字节, 继续收 */
        uint8_t cmd = g_frame[2];
        if (cmd > 2) {
            g_frameLen = 0;           /* CMD 非法, 重新同步 */
            continue;
        }
        /* 帧长: 统一 5 字节 [AA][ID][CMD][目标/填充][SUM] — 无父角
         * (父臂/顺齐偏移全走 ADC 实测/世界角闭环, 帧第4字节: 写=世界目标, 读/急停=忽略) */
        uint8_t needLen = 5;
        if (g_frameLen < needLen)
            continue;
        uint8_t id   = g_frame[1];
        uint8_t data = g_frame[3];
        uint8_t sum  = g_frame[needLen - 1];
        g_frameLen = 0;

        /* 校验和: 除校验字节外全部累加 & 0xFF */
        uint8_t calc = 0;
        for (int i = 0; i < needLen - 1; i++)
            calc += g_frame[i];
#if (DIAG_EN == 1)
        /* 帧日志: 内容 + 校验结果 (定位收不到应答/电机不动) */
        DBG_PRINTF("rx[%u]:", (unsigned int)needLen);
        for (int i = 0; i < needLen; i++)
            DBG_PRINTF(" %02X", g_frame[i]);
        DBG_PRINTF("%s\r\n", (calc == sum) ? " OK" : " FAIL");
#endif
        if (calc != sum || ((id != 0x01 || cmd != 0) && data > 180))
            continue;                 /* 校验失败/非法字段, 丢弃 */

        /* ② 急停 (CMD 02): 广播, 本板所有电机停止 (不受 ID/跳线限制), 不应答
         *    无刷: Bldc_Stop = PWM 归零 + BK 锁止 + 状态复位
         *    步进: 关 APT 脉冲 + EN 关断 + SLEEP 休眠 */
        if (cmd == 2) {
#if (DIAG_EN == 1)
            DBG_PRINTF("rx: STOP all\r\n");
#endif
            for (int i = 0; i < 3; i++) {
                if (Bus_IsMyMotor((uint8_t)(i + 2)))
                    Bldc_Stop(&g_motor[i]);
            }
            Stepper_Stop(&g_motor1);
            continue;
        }

        /* ③ 跳线识别: 本板只响应跳线对应的电机 ID (00→01, 01→02+03, 10→04) */
        if (!Bus_IsMyMotor(id)) {
#if (DIAG_EN == 1)
            DBG_PRINTF("rx: id=%u no (board=%u)\r\n", (unsigned int)id, (unsigned int)g_boardId);
#endif
            continue;                 /* 不是本板电机, 只收不应答 (TX 保持 GPIO 高阻) */
        }
        switch (cmd) {
        case 1:   /* 读 → 只上报角度, 然后等待 */
#if (DIAG_EN == 1)
            DBG_PRINTF("rx: m%u read\r\n", (unsigned int)id);
#endif
            if (id == 0x01)
                Bus_Reply(id, Motor1_Angle());                  /* 步进: 局部角 */
            else
                Bus_Reply(id, Bus_WorldAngle(id, 0.0f));        /* 世界角 (缓存) */
            break;
        case 0:   /* 写 → data 即目标 (无父角, 钳位 0~180) */
            if (id == 0x01) {
                /* 电机1 (步进板 跳线00) */
                /* 无电位器测试：收到电机1写指令后固定输出500个脉冲。 */
                /* data: bit7=direction, bit6..0=pulse count (1..127). */
                unsigned int steps = (unsigned int)(data & 0x7FU);
                unsigned int dir = (unsigned int)((data >> 7) & 0x01U);
                if (steps != 0U)
                    Stepper_Run(&g_motor1, steps, dir);
            } else if (id == 0x02) {
                /* 电机2 (肩, 双无刷板 跳线01): 世界目标 (底座父臂=0) */
                Bus_ServoWrite(0x02, data);
            } else if (id == 0x03) {
                /* 电机3 (肘, 双无刷板): 世界目标 (世界角闭环, 含 M2 实测) */
                Bus_ServoWrite(0x03, data);
            } else {
                /* 电机4 (04 单关节板 跳线10): 独立关节目标 */
                Bus_ServoWrite(0x04, data);
            }
            break;
        default:
            break;
        }
    }
}

/* ================================================================
 *  main
 * ================================================================ */
int main(void)
{
    int i;

    SystemInit();
    DBG_UartPrintInit(115200);
    DBG_PRINTF("3board cascade pid servo\r\n");

    HAL_GPT_Start(&g_gptHandle0);
    HAL_GPT_Start(&g_gptHandle1);

    UART_Ring_Init();
    UartBus_Init();   /* 总线地址脚 (GPIO2_7/GPIO3_0) + TX 默认切 GPIO (不驱动总线) */

    /* 本板地址 (跳线 GPIO2_7/GPIO3_0): 决定本板响应哪些电机 (00→04, 01→02+03, 10→01) */
    // g_boardId = UartBus_GetAddr();
    g_boardId=0x02;
    DBG_PRINTF("board id=%u\r\n", (unsigned int)g_boardId);

    /* 上电解锁: 本驱动 IC 上电 BK 低会进保护锁存, FR 翻转一次清掉, 否则开环起步转不动 */
    for (i = 0; i < 3; i++) {
        if (!Bus_IsMyMotor((uint8_t)(i + 2)))
            continue;
        Bldc_FaultClear(&g_motor[i]);
    }
    DBG_PRINTF("ready\r\n");

    while (1) {
        /* 5ms 速度环: 本板电机 堵转检测 → 测速 → 速度 PI → 输出落硬件 */
        if (g_spdFlag) {
            g_spdFlag = 0;
            for (i = 0; i < 3; i++) {
                if (!Bus_IsMyMotor((uint8_t)(i + 2)))
                    continue;                 /* 只跑本板电机 (跳线分板) */
                Motor_Bldc *m = &g_motor[i];
                /* 无 FG 归零: 电机停后 rpmFb 冻结在旧值 → err 恒负 → kp·err 压输出 → 推不动.
                 * 200ms 无 FG 边沿 → 转速归零 (正常转时 FG 每 ~48ms 一个边沿, 不误归零) */
                if (m->fb.initialized && m->fb.rpm > 0.0f && !m->fb.updated) {
                    if (++g_noFg[i] >= 40) {
                        m->fb.rpm = 0.0f;
                        g_noFg[i] = 0;
                    }
                } else {
                    g_noFg[i] = 0;
                }
                BldcFb_Update(m);
                /* 重力补偿用世界角: g_degS 已按各电机存世界角
                 * (M3 的 θ3w=θ2+θ3−90 由位置环每拍算好, 含 M2 实测) */
                float thw = g_degS[i] * 0.0174533f;
                BldcSpd_Run(m, 0.005f, thw);
                /* 方向适配: 实测 FR LOW=减角 (14:59 铁证: 增角指令下 FR LOW 电机减角),
                 * 增角需 FR HIGH — userPID完成 原样 out≥0→FR LOW 在当前硬件取反 */
                Bldc_SetOutput(m, m->spd.duty, (uint8_t)(1 - m->spd.dir));
            }
        }
        /* 40ms 位置环: 显式读本板电机 ADC → 三点标定 → 位置 P → 速度目标
         * 双无刷板 (跳线01): 同拍先 M2 (ADC3) 后 M3 (ADC2) — M3 世界角用刚刷新的 M2
         *   (速度环的 θ3w=θ2+θ3−90 依赖此顺序, 双轴互注同拍无延迟)
         * 04板 (跳线00): M4 独立关节 (ADC3, 父臂=M3 写死 M4_M3_WORLD_DEG)
         * 步进板 (跳线10): 无无刷位置环 */
        if (g_posFlag) {
            g_posFlag = 0;
            g_posRun++;
            for (i = 0; i < 3; i++) {
                if (!Bus_IsMyMotor((uint8_t)(i + 2)))
                    continue;
                Motor_Bldc *ms = &g_motor[i];
                if (fabsf(ms->spd.cmdRpm) > 1.0f && !ms->fb.updated) {
                    if (++g_stallCnt[i] >= 50) {   /* 20ms×50=1s */
                        GPT_ReferCfg ref = {.refA0 = {.refdot = 0, .refAction = GPT_ACTION_OUTPUT_HIGH},
                                            .refB0 = {.refdot = 0, .refAction = GPT_ACTION_NO_ACTION}};
                        if (ms->pwm)
                            HAL_GPT_SetReferCounterAndAction(ms->pwm, &ref);   /* PWM 全关 */
                        if (ms->frGpio)
                            HAL_GPIO_SetValue(ms->frGpio, ms->frPin, GPIO_HIGH_LEVEL);  /* FR 翻转 */
                        BASE_FUNC_DELAY_MS(50);
                        if (ms->frGpio)
                            HAL_GPIO_SetValue(ms->frGpio, ms->frPin, GPIO_LOW_LEVEL);
                        g_stallCnt[i] = 0;
                    }
                } else {
                    g_stallCnt[i] = 0;
                }
            }
            uint16_t v;
            float deg;
            Motor_Bldc *m;
            /* M2 (肩): 板内 ADC3 实测 → 局部 = 世界 (底座父臂=0) — EMA 平滑抗 ADC 噪声 */
            if (Bus_IsMyMotor(0x02)) {
                m = &g_motor[0];
                ADC_Read(&g_adc3, &v);
                g_adcRaw[0] = v;
                deg = AngleToDeg(&g_cal[0], v);
                g_degS[0] = g_degS[0] * 0.9f + deg * 0.1f;   /* EMA 0.9/0.1 */
                PosLoopOne(m, 0);
            }
            /* M3 (肘): ADC2 → 局部 → 世界角 θ3w = M2(刚刷新) + θ3 − 90 */
            if (Bus_IsMyMotor(0x03)) {
                m = &g_motor[1];
                ADC_Read(&g_adc2, &v);
                g_adcRaw[1] = v;
                deg = AngleToDeg(&g_cal[1], v);
                g_degS[1] = g_degS[1] * 0.9f + (g_degS[0] + deg - g_aligned[1]) * 0.1f;   /* EMA 0.9/0.1 */
                PosLoopOne(m, 1);
            }
            /* M4 (04板): ADC3 → 独立关节, 世界角 = M3 写死 90° + 局部 − 90 (=局部) — EMA 0.7/0.3
             * (04 板载 ADC0 有问题, 电位器实际走 ADC3) */
            if (Bus_IsMyMotor(0x04)) {
                m = &g_motor[2];
                ADC_Read(&g_adc3, &v);
                g_adcRaw[2] = v;
                deg = AngleToDeg(&g_cal[2], v);
                g_degS[2] = g_degS[2] * 0.7f + (M4_M3_WORLD_DEG + deg - 90.0f) * 0.3f;
                PosLoopOne(m, 2);
            }
        }
        /* 串口: 总线帧 (0xAA 包头) */
        UartPump();
#if (DIAG_EN == 1)
        /* 遥测: 每 200ms — 电机状态 (cur/tgt + kp/ki/积分/actRpm/duty/cmd/rpm) */
        if (g_telFlag) {
            g_telFlag = 0;
            for (i = 0; i < 3; i++) {
                if (Bus_IsMyMotor((uint8_t)(i + 2))) {
                    Motor_Bldc *m = &g_motor[i];
                    DBG_PRINTF("m%u cur=%.1f tgt=%.1f adc=%u duty=%u rpm=%.0f\r\n",
                               (unsigned int)(i + 2), g_degS[i], m->pos.target,
                               g_adcRaw[i], m->spd.duty, m->fb.rpm);
                }
            }
        }
#endif
        BASE_FUNC_DELAY_MS(1);
    }
}
