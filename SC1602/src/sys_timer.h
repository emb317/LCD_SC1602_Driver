
#ifndef _SYS_TIMER_H_
#define _SYS_TIMER_H_

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

#include <cr_section_macros.h>

void SysTimer_Init(void);
void SysTimer_WaitMs(uint16_t msec);

#endif /* _SYS_TIMER_H_ */
