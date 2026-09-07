#ifndef INC_SYSTEM_H
#define INC_SYSTEM_H

#include "common-defines.h"

#define CPU_FREQ     (84000000)   //system clock: 84MHz
#define SYSTICK_FREQ (1000)       //Systick frequency: 1000Hz or 1ms per tick

void system_setup(void);
uint64_t system_get_ticks(void);

#endif // INC_SYSTEM_H
