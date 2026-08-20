# 海思 MCU SDK 分析报告

## 概述

本SDK是**海思（HiSilicon）SolarA2 系列 MCU 的专用软件开发套件**，版本 `SolarA2_1.1.0.13`。它是一个面向**电机控制和数字电源**领域的嵌入式 SDK，并非通用的 MCU 开发框架。

---

## 一、PID 算法分析

### ✅ SDK **完整包含** PID 算法

SDK 实现了完整的 PID 控制器，代码位于：

```
middleware/control_library/pid_controller/
├── mcs_pid_ctrl.h    # PID 控制器头文件
└── mcs_pid_ctrl.c    # PID 控制器实现
```

### PID 实现特性

| 特性 | 描述 |
|------|------|
| **PID_Handle 结构体** | 完整的 PID 状态结构，包含误差、积分、微分、前馈、抗饱和等全部成员 |
| **PI_Exec()** | 简化 PI 控制器：比例+积分+静态钳位+前馈，无微分项 |
| **PID_Exec()** | 完整 PID 控制器：比例+积分+微分+**动态抗饱和（back-calculation）**+前馈 |
| **PID_ExecDiffWithFbk()** | PID + 微分项基于反馈值计算（**抗微分冲击/derivative kick**），避免设定值突变引起的微分冲击 |
| **参数设置 API** | `PID_SetKp/Ki/Kd/Ns/Ts/Limit()` 支持在线调参 |
| **微分项滤波** | 通过 `ns` 参数对微分项进行低通滤波，抑制高频噪声 |
| **抗积分饱和** | 通过 `ka` 参数+动态钳位的 back-calculation 机制实现 |
| **输出限幅** | `upperLimit/lowerLimit` 对最终输出进行钳位 |

### PID 在 SDK 中的使用场景

PID 控制器被广泛用于上层控制模块：

| 模块 | 文件 | PID 用途 |
|------|------|----------|
| **电流环控制** | `mcs_curr_ctrl.h/c` | d/q 轴各一个 PI 控制器，控制电机电流 |
| **速度环控制** | `mcs_spd_ctrl.h/c` | 一个 PI 控制器，控制电机转速 |
| **位置环控制** | `mcs_pos_ctrl.h/c` | 一个 PID 控制器（含七段轨迹规划），控制电机位置 |
| **PFC 电压环** | `pfc_volt_ctrl.h/c` | PI 控制器，控制 PFC 母线电压 |
| **PFC 电流环** | `pfc_curr_ctrl.h/c` | PI 控制器，控制 PFC 电流 |
| **PLL 锁相环** | `mcs_pll.h/c` | PI 控制器用于锁相环角度/速度估算 |
| **IF 控制** | `mcs_if_ctrl.h/c` | PI 控制器用于电流-频率启动控制 |

---

## 二、SDK 通用性分析

### 结论：**非通用 SDK，是专用电机/电源控制 SDK**

### 定位

这是一个面向**海思 SolarA2 系列 MCU**（Cortex-M 系列）的**电机控制与数字电源专用 SDK**。

### 支持的芯片系列

```
chip/
├── 3061h / 3061m           # 单核系列
├── 3065a / 3065h / 3065p   # 中端系列
├── 3066h / 3066m           # 高端多核系列（含双核/三核，支持锁步）
├── 3067m                   # 最新多核系列
└── 3071                    # 最新系列
```

### 硬件加速特性

- **CLA（Control Loop Accelerator）**：硬件控制环加速器，支持硬件自动执行 PID + 前馈控制，无需 CPU 干预
- **MAU（Math Acceleration Unit）**：硬件数学加速单元，加速三角函数/除法/平方根
- **DPWM**：专用数字 PWM 模块，支持 LLC/PFC/全桥等多种电源拓扑
- **APT**：高级 PWM 定时器，支持单/三电阻电流采样触发
- **EADC**：增强型 ADC，与 DPWM 同步触发

### 中间件控制算法库

```
middleware/control_library/
├── pid_controller/    # PID/PI 控制器
├── foc_loop_ctrl/     # FOC 电流/速度/位置环 + 弱磁 + IF + 启动
├── observer/          # 滑模观测器 (SMO) 无感估算转子位置
├── filter/            # 低通滤波/陷波/PLL 锁相环
├── modulation/        # SVPWM / R1-SVPWM 调制
├── pfc/               # PFC 电压/电流环控制
├── math/              # 数学库（Clarke/Park/三角函数/Sqrt）
├── ramp/              # 斜坡管理（加减速控制）
├── protection/        # 缺相/堵转/不平衡检测
├── brake/             # 制动控制
├── vf/                # V/F 标量控制
├── power/             # 功率管理
├── adc_calibr/        # ADC 校准
└── utilities/         # 电机参数/类型定义/系统状态
```

---

## 三、速度检测方法：T 算法 vs M 算法

### 结论：所有 Sample 统一使用 **T 算法**（周期测量法）

SDK 中没有使用 M 算法（固定窗口脉冲计数）的 sample。所有速度估算都基于测量**传感器事件之间的时间间隔**来计算速度。

| 维度 | M 算法（脉冲计数） | T 算法（周期测量） |
|------|-------------------|-------------------|
| **原理** | 固定时间窗口内计脉冲数 | 测量固定脉冲数之间的时间 |
| **高速精度** | ✅ 高 | ❌ 低 |
| **低速精度** | ❌ 低 | ✅ 高 |
| **响应速度** | 慢（等窗口结束） | 快（每脉冲更新一次） |
| **SDK 选择** | **未使用** | ✅ **全部使用** |

---

## 四、当前用户实现：双路 BLDC 电机 PID 速度控制

### 4.1 硬件拓扑

```
┌──────────────────────────────────────────────────────────────┐
│                     电机1 (原板)                              │
│  PWM (20kHz) ◀────── GPT0 / GPIO7_0 PIN14                    │
│  FR  方向    ◀────── GPIO2_2 PIN10                           │
│  BRAKE 刹车  ◀────── GPIO4_6 PIN11 (LOW=锁)                  │
│  FG  反馈    ──────▶ GPIO6_2 (下降沿中断)                     │
└──────────────────┬───────────────────────────────────────────┘
                   │
┌──────────────────┴───────────────────────────────────────────┐
│                     电机2 (新增)                              │
│  PWM (20kHz) ◀────── GPT1 / GPIO7_1                          │
│  FR  方向    ◀────── GPIO4_7                                 │
│  BRAKE 刹车  ◀────── GPIO6_7 (LOW=锁)                        │
│  FG  反馈    ──────▶ GPIO5_3 (下降沿中断)                     │
└──────────────────┬───────────────────────────────────────────┘
                   │
┌──────────────────┴───────────────────────────────────────────┐
│           SolarA2 3066H MCU                                   │
│  ● GPT0/GPT1 20kHz PWM, 10000步分辨率                          │
│  ● 两路 FG T法测速 + EMA低通滤波                                │
│  ● SDK PI_Exec 双路速度闭环（句柄实例化）                        │
│  ● SDK RMG_Exec 速度斜坡                                       │
│  ● 事件驱动 PID (每FG脉冲执行一次)                               │
│  ● BRAKE 引脚低电平锁止                                         │
└───────────────────────────────────────────────────────────┘
```

### 4.2 引脚定义

| 引脚 | 功能 | 逻辑 |
|------|------|------|
| **GPT0 (PIN14)** | 电机1 PWM 速度指令 | 20kHz, 低电平有效, duty 0-9999 |
| **GPIO2_2 (PIN10)** | 电机1 FR 方向 | LOW=正转, HIGH=反转 (实测硬件极性) |
| **GPIO4_6 (PIN11)** | 电机1 BRAKE 刹车 | LOW=锁止, HIGH=释放 |
| **GPIO6_2** | 电机1 FG 速度反馈 | 6脉冲/圈, 下降沿中断 |
| **GPT1 (GPIO7_1)** | 电机2 PWM 速度指令 | 20kHz, 低电平有效, duty 0-9999 |
| **GPIO4_7** | 电机2 FR 方向 | LOW=正转, HIGH=反转 (实测硬件极性) |
| **GPIO6_7** | 电机2 BRAKE 刹车 | LOW=锁止, HIGH=释放/转动 |
| **GPIO5_3** | 电机2 FG 速度反馈 | 6脉冲/圈, 下降沿中断 |
| **GPIO0_5** | 菜单键 | 上升沿中断；按一下切到电机1/电机2 |
| **GPIO0_7** | 正转/停止键 | 上升沿中断；第一次正转，第二次停止，循环 |
| **GPIO16_7** | 反转/停止键 | 上升沿中断；第一次反转，第二次停止，循环 |

### 4.3 控制状态机

```
     按键FWD/REV                  filterInitialized
STOP ──────────▶ OPENLOOP(50%开环) ─────────────▶ CLOSEDLOOP(PI闭环)
  ▲                                                   │
  │◀────── SetTarget(0) ──────────────────────────────┘
  │         (保持 CLOSEDLOOP，UP 方向 + 积分×0.3 + bump 制衡)
  │
  └──── 按键FWD/REV (释放BRAKE, 重新OPENLOOP)
```

4 个状态：

| 状态 | 触发 | 行为 |
|------|------|------|
| **STOP** | 上电/停车 | BRAKE=LOW, PWM=0 |
| **OPENLOOP** | 按键启动 | 固定 50% 占空比, 等 FG 数据有效 |
| **CLOSEDLOOP** | filterInitialized | RMG斜坡 + PI_Exec 闭环调速 / 停车保持 |
| **BRAKE** | 看门狗/急停 | BRAKE=LOW, PWM=0 |

### 4.4 FG 速度反馈

纯 T 法：`RPM = 60 / (6 * dt)`（dt = 相邻 FG 脉冲时间间隔，电机 12 极/6 对极 → Z=6 脉冲/转）。

- FG GPIO ISR 记录时间戳（TIMER2 自由运行定时器 `BASE_FUNC_GetTick()`）
- `SpeedFb_Update()` 在主循环调用：T 法算瞬时 RPM → EMA 低通滤波（fc=5Hz）
- 野值剔除：\|raw - filtered\| > 1500 RPM → 丢弃，连续 3 次 → 强制恢复
- 无 FG 时保持上一个滤波值（不衰减到 0）
- 首样本保护：跳过第一个 FG 边沿（prevEdgeTime=0 → dt 无意义）

### 4.5 停止机制（堵转 bump 制衡式保持）

按 STOP → `SpeedCtrl_SetTarget(0)`（保持 CLOSEDLOOP，不切 BRAKE）：
1. 强制 REV(UP) 方向，积分砍到 30% 快速卸掉驱动力
2. 保持期 error 恒=0，PI 冻结，输出=积分（FG 无方向：error=-fbRPM 会把下滑误判为上转而卸力——实测 6s 流失 431→217；error=+fbRPM 会把 bump 回弹误判为滑移而助力——位置修正全部交给 bump）
3. 位置维持靠主循环堵转恢复：无 FG 100ms → bump 抬占空比，每级独占完整 100ms 窗口（bumpHoldCnt 期间 PI 不改写占空比）；FG 一动即刻交还 PI。**bump 加 PWM、PI 守住基准力、重力拉回，三者制衡**（持续小幅抖动是机制本身——驱动 IC ~1.1s 堵转自锁，必须周期性真动才能保命）
4. 仅急停/看门狗才 `Motor_Brake(motor, 1)` 机械锁止 + PWM=0

### 4.6 PID 控制特性（CLOSEDLOOP）

- **RMG 速度斜坡**：目标速度平滑变化，无阶跃冲击
- **PI_Exec**：误差 = targetSmooth - fbRPM，输出映射到 [0, dutyMax]
- **反饱和**：积分接近 ±90% 限幅时冻结 ki
- **积分步进限制**：单步最大 500 计数（5% duty）
- **过零清积分**：连续 2 次误差穿越 (>200 RPM) → 清积分
- **死区**：输出 < 200 → 强制为 0

### 4.7 主循环架构（事件驱动）

| 优先级 | 中断 | 驱动 |
|--------|------|------|
| 0 | FG GPIO ISR (GPIO6_2/GPIO5_3) | `BldcFb_Isr()`, 记录时间戳 |
| 1 | Timer1 ISR (5ms) | `g_isrTick++` (公共 5ms 时基) + `Key_TimerTick()` 按键消抖/执行 |
| 最低 | Main loop | 按键处理 → 对每台电机跑 `SpeedCtrl_Run` / 堵转恢复 / FG 看门狗 |

```
OPENLOOP:   每 5ms → SpeedCtrl_Run (ts=5ms固定)
CLOSEDLOOP: fgCtrlFlag[i](FG脉冲) → SpeedCtrl_Run (dt=实际脉冲间隔)
            target=0 时额外每 5ms 跑一次保持环
STOP/BRAKE: 不跑速度环
```

### 4.8 保护机制

| 机制 | 条件 | 行为 |
|------|------|------|
| 堵转恢复 | CLOSEDLOOP 中 100ms 无 FG（含停车保持期） | 逐级抬占空比 20% 起步，每 100ms +10%，上限 100%；保持期每级 bump 独占 100ms 窗口（bumpHoldCnt），FG 一动即交还 PI。保持期：bump 加力、PI 守基准、重力拉回，三者制衡 |
| FG 看门狗 | OPENLOOP/CLOSEDLOOP 中 1s 无 FG | EmergencyStop → BRAKE |
| EmergencyStop | 看门狗超时 | BRAKE=LOW + PWM=0 + 打印 |

### 4.9 SDK 模块复用

| SDK模块 | 文件 | 用途 |
|---------|------|------|
| `PI_Exec()` | `mcs_pid_ctrl.h` | 速度环 PI 控制器 |
| `RMG_Exec()` | `mcs_ramp_mgmt.h` | 速度斜坡（平滑加减速） |
| `PID_Reset/Clear` | `mcs_pid_ctrl.h` | PID 状态重置 |

### 4.10 文件结构

```
user/
├── main.c                      # 主循环 + 串口帧解析 + 4台电机显式调度
├── generatecode/               # SDK 生成, 不动
│   ├── main.h                  # SDK 句柄声明, 中断回调声明
│   ├── feature.h               # 芯片宏开关, 外设配置
│   └── system_init.c           # 时钟/IO/外设初始化
├── motors/
│   ├── motor_stepper.h/.c      # 步进电机结构体 + Stepper_Run/Stop/RunToAngle
│   ├── motor_bldc.h/.c         # BLDC 结构体 + 硬件操作/FG测速/速度PI/位置环P
├── pid/
│   ├── pid.h/.c                # PID 通用结构体 + API
├── adc/
│   ├── adc_user.h/.c           # ADC 读取接口
└── uart/
    ├── uart_ring.h/.c          # UART 环形缓冲 + SDK 中断回调
```

### 4.11 按键操作（定时器延时消抖）

GPIO 按键中断只负责“触发”，真正的消抖延时在 5ms 定时器中断里完成：
- 触发后启动 10ms 倒计时（2 个 5ms tick），到点读引脚确认高电平才执行动作；低电平视为抖动丢弃
- 执行动作后进入 15ms 释放确认（3 个 5ms tick），读到低电平才重新武装，防止一次按键多次触发

按键逻辑（操作当前选中的电机，菜单键循环选电机1/电机2）：

- GPIO0_5 菜单键：第一次选中电机1，第二次选中电机2，循环
- GPIO0_7 正转/停止键：第一次正转（FR=LOW, target=2000RPM），第二次停止，循环
- GPIO16_7 反转/停止键：第一次反转（FR=HIGH, target=2000RPM），第二次停止，循环
- 两键互斥：当前处于正转时按反转键会切到反转；当前处于反转时按正转键会切到正转
- 仅 FG 看门狗/急停才会触发 BRAKE 机械锁止

---

## 五、总结

| 维度 | 结论 |
|------|------|
| **SDK 是否通用** | ❌ 海思 SolarA2 MCU 电机/电源专用 SDK |
| **PID 是否通用** | ✅ PID 模块可独立剥离到其他平台 |
| **当前实现** | 四轴控制：步进1 + BLDC2 + BLDC3 + 步进4，共用一套代码句柄实例化 |
| **停止方式** | Bldc_Stop (PWM=0+BRAKE=LOW) / Hold 模式 (target≈0 积分+重力保位) |
| **速度反馈** | FG T 法 (6脉冲/圈) + EMA 低通滤波 |
| **BLDC 启动方式** | 开环起步 → FB 有效后切 PI 闭环 |
| **堵转恢复** | Bldc_FaultClear — FR 反转清驱动 IC 保护 |
| **保护** | FG 看门狗 |
