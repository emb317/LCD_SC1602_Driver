
#include "sc1602.h"

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

#include "sys_timer.h"

void SC1602_WaitUs(uint32_t t) __attribute__ ((naked));
void SC1602_WaitUs(uint32_t t)
{
	asm("	MOV r1, #6	");
	asm("	MUL r0, r1	");
	asm("1:				");
	asm("	BEQ 1f		");
	asm("	SUB r0, #1	");
	asm("	B 1b		");
	asm("1:				");
	asm("	BX lr		");
}

void SC1602_WaitMs(uint32_t t)
{
	SysTimer_WaitMs(t);
}

