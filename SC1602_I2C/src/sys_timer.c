#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

#include "sys_timer.h"

#include <cr_section_macros.h>

//受け取った値[ms]の間ウェイト
void SysTimer_WaitMs(uint16_t msec)
{
	uint16_t i;
	SysTick->CTRL |= (1<<0);
	for(i=0; i<msec; i++)
	{
		while(!((SysTick->CTRL>>16)&0x01))
		{
		}
	}
	SysTick->CTRL  &= ~(1<<1);
}

void SysTimer_Init(void)
{
	//SysTickTimerの初期化
	SysTick->LOAD  = (24000 - 1);
	SysTick->VAL   = 0;
}
