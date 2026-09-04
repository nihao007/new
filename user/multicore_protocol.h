#ifndef MULTICORE_PROTOCOL_H
#define MULTICORE_PROTOCOL_H

/*
 * CPU0 -> CPU1/CPU2 mailbox layout.  The first three words keep the
 * external UART order [ID][CMD][DATA]; DATA3 is an internal sequence.
 */
#define MC_MOTOR_ID_PING          0U
#define MC_MOTOR_ID_2             2U
#define MC_MOTOR_ID_3             3U

#define MC_CMD_MOVE               0U
#define MC_CMD_QUERY              1U
#define MC_CMD_STOP               2U
#define MC_CMD_PING               0xFFFFFFFEU

#define MC_RESULT_OK              0U
#define MC_RESULT_WRONG_ID        1U
#define MC_RESULT_BAD_COMMAND     2U
#define MC_RESULT_BAD_VALUE       3U
#define MC_RESULT_BAD_PING        4U

#define MC_PING_MAGIC             0x50494E47U

#endif
