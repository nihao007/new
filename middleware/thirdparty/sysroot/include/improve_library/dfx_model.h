/**
 * @copyright Copyright (c)  HiSilicon (Shanghai) Technologies  Co., Ltd. 2025-2025. All rights reserved.
 * @file    dfx_model.h
 * @author  MCU Driver Team
 * @brief   header file of model dfx func
 */
#ifndef DFX_MODEL_H
#define DFX_MODEL_H

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

#ifndef VA_START
#define VA_START(v, l)  __builtin_va_start(v, l)
#endif
#ifndef VA_ARG
#define VA_ARG(v, l)    __builtin_va_arg(v, l)
#endif
#ifndef VA_END
#define VA_END(v)       __builtin_va_end(v)
#endif
typedef __builtin_va_list va_list;

void PrintArray(float *array, unsigned int len, char* name);
void Print2DArray(float *array, unsigned int ro, unsigned int co, char* name);
void PrintDebugLog(const char *format, ...);

#ifndef RELEASE
#define AssertElseHaltM(cond)  do {if (!(cond)) {while (1);}} while (0)
#define AssertElseHaltAndPrintM(cond, msg)  do {if (!(cond)) {while (1);}} while (0)
#define AssertElseReturnM(cond, ret_code)  do {if (!(cond)) {return (ret_code);}} while (0)
#define AssertElsePrintM(cond, ret_code, msg)   do {if (!(cond)) {return (ret_code);}} while (0)
#define AssertCheckM(cond)   AssertElseReturnM((cond), -1)
#define AssertReturnM(cond)   do {if (!(cond)) {return ;}} while (0)
#define AssertSuccessM(ret)  AssertElseReturnM(((ret) == 0), -1)
#define CheckElsePrintM(cond, msg)  do {if (!(cond)) {}} while (0)
#define CheckElseWarnM(cond)  do {if (!(cond)) {}} while (0)
#define PrintDebugLogM(...)    do {PrintDebugLog(__VA_ARGS__);} while (0)
#else
#include "debug.h"
#define printd(...) DBG_PRINTF(__VA_ARGS__)
#define AssertElseHaltM(cond) \
        do { \
            if (!(cond)) { \
                printd("file %s:%s() line %d assert fail\n", __FILE__, __FUNCTION__, __LINE__); \
                while (1); \
            } \
        } while (0)
#define AssertElseHaltAndPrintM(cond, msg) \
        do { \
            if (!(cond)) { \
                printd("file %s:%s() line %d assert fail: %s\n", __FILE__, __FUNCTION__, __LINE__, msg); \
                while (1); \
            } \
        } while (0)
#define AssertElseReturnM(cond, ret_code) \
        do { \
            if (!(cond)) { \
                printd("file %s:%s() line %d assert fail\n", __FILE__, __FUNCTION__, __LINE__); \
                return (ret_code); \
            } \
        } while (0)
#define AssertElsePrintM(cond, ret_code, msg) \
        do { \
            if (!(cond)) { \
                printd("file %s:%s() line %d assert fail: %s\n", __FILE__, __FUNCTION__, __LINE__, msg); \
                return (ret_code); \
            } \
        } while (0)
#define AssertCheckM(cond)   AssertElseReturnM((cond), -1)
#define AssertSuccessM(ret)  AssertElseReturnM(((ret) == 0), -1)
#define CheckElsePrintM(cond, msg)  do {if (!(cond)) {printd(msg);}} while (0)
#define CheckElseWarnM(cond) \
        do { \
            if (!(cond)) { \
                printd("WARNING: file %s:%s() line %d check fail!\n", __FILE__, __FUNCTION__, __LINE__); \
            } \
        } while (0)
#define PrintDebugLogM(...)    do {printd(__VA_ARGS__);} while (0)
#endif

#endif