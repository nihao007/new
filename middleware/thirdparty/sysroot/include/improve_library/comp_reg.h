/**
 * @copyright Copyright (c)  HiSilicon (Shanghai) Technologies  Co., Ltd. 2025-2025. All rights reserved.
 * @file    comp_reg.h
 * @author  MCU Driver Team
 * @brief   header file of flash and ram API register
 */
#ifndef FLASH_RAM_REG_H
#define FLASH_RAM_REG_H
#include <stdbool.h>
#include <stdint.h>
/* ram base operation */
typedef struct {
    int (*initSram)(void);
    int (*writeSram)(uint32_t psramAddr, uint8_t* fromBuffer, uint32_t dataLen);
    int (*readSram)(uint32_t psramAddr, uint8_t* toBuffer, uint32_t dataLen);
} HalRamAPI;

/* flash base information */
typedef struct {
    uint32_t pageNum;
    uint32_t eraseMinSize;
    uint32_t flashSize;
} FlashInfo;

/* flash base operation */
typedef struct {
    int (*initFlash)(void);
    int (*eraseFlashChip)(void);
    int (*eraseFlashMinUnit)(uint32_t addr);
    int (*writeFlash)(uint32_t addr, uint8_t* fromBuffer, uint32_t writeLen, bool verifyAfterWrite);
    int (*readFlash)(uint32_t addr, uint8_t* toBuffer, uint32_t readLen);
    int (*getFlashInfo)(FlashInfo* info);
} HalFlashAPI;

/* system tick and core frequency */
typedef struct {
    unsigned int (*getTick)(void);
    unsigned int (*getCoreFreq)(void);
} UtilInitConfig;

void HalRamAPIRegistration(HalRamAPI ramApi);    /* ram api register */
void HalFlashAPIRegistration(HalFlashAPI flashApi);  /* flash api register */

int UtilModeInit(UtilInitConfig config);      /* system tick and core frequency register */

typedef void (*PrintBuffer)(float *array, unsigned int len, char* name);  /* 1-D array print */
/* 2-D array print */
typedef void (*PrintRankBuffer)(float *array, unsigned int row, unsigned int col, char* name);
typedef void (*DebugLog)(const char *format, ...);  /* base dfx print */

void RegisterDfxPrintBuffer(PrintBuffer arrFunc);    /* register for 1-d print func */
void RegisterDfxPrintRankBuffer(PrintRankBuffer arrFunc);   /* register for 2-d print func */
void RegisterDfxDebugLog(DebugLog debugFunc);        /* register for base dfx print func */

#endif