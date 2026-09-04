#ifndef I2C_BUS_H
#define I2C_BUS_H

#include <stdint.h>
#include "typedefs.h"

#define I2C_BUS_FRAME_SIZE 6U
#define I2C_BUS_REPLY_MAX_SIZE 5U

/*
 * Snapshot of the I2C0 slave state used during board bring-up.  Interrupt
 * callbacks only update counters; formatting and UART output stay in task
 * context so that debug output cannot disturb I2C timing.
 */
typedef struct {
    uint32_t busActive;
    uint32_t sclLevel;
    uint32_t sdaLevel;
    uint32_t reinitCount;
    uint32_t startCount;
    uint32_t addressMatchCount;
    uint32_t ackUnmatchCount;
    uint32_t sclTimeoutCount;
    uint32_t stopCount;
    uint32_t shortStopCount;
    uint32_t rxCount;
    uint32_t txCount;
    uint32_t errorCount;
    uint32_t lastRawStatus;
    uint32_t lastRxAddress;
    uint32_t lastRxDirection;
    uint32_t driverState;
    uint32_t ownAddressRegister;
    uint32_t interruptEnableRegister;
    uint32_t sclTimeoutRegister;
    uint32_t fsmStatusRegister;
} I2cBusDiag;

/*
 * Prepare I2C0 as a 7-bit slave on PIN72/PIN71.  The peripheral is started
 * later by I2cBus_Service() after SCL and SDA are both stably high.
 * The temporary address convention is 0x20 + boardId.
 */
BASE_StatusType I2cBus_Init(uint8_t boardId);

/*
 * Copy one completed 6-byte write transaction into frame.
 * Returns 1 when a frame was copied, otherwise returns 0.
 */
uint8_t I2cBus_GetFrame(uint8_t frame[I2C_BUS_FRAME_SIZE]);

/* Byte-stream access used by the existing UART protocol parser. */
uint8_t I2cBus_HasData(void);
uint8_t I2cBus_ReadByte(void);

/*
 * Copy a reply into the slave TX mailbox.  The data is shifted out only when
 * the I2C master starts a read transaction.
 */
BASE_StatusType I2cBus_PrepareReply(const uint8_t *data, uint8_t length);

/* Advance deferred TX/RX state outside the interrupt callback. */
void I2cBus_Service(void);

uint8_t I2cBus_GetSlaveAddress(void);
uint32_t I2cBus_GetRxCount(void);
uint32_t I2cBus_GetTxCount(void);
uint32_t I2cBus_GetErrorCount(void);
void I2cBus_GetDiag(I2cBusDiag *diag);

#endif
