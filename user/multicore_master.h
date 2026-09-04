#ifndef MULTICORE_MASTER_H
#define MULTICORE_MASTER_H

#include "typedefs.h"

BASE_StatusType MultiCoreMaster_InitAndStart(void);
BASE_StatusType MultiCoreMaster_SendMotorCommand(unsigned int motorId,
                                                 unsigned int command,
                                                 unsigned int value);
void MultiCoreMaster_Service(void);

#endif
