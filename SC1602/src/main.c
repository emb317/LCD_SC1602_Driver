
#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

#include <cr_section_macros.h>

#include "sc1602.h"
#include "sys_timer.h"

int main(void)
{
	int i;
	int j = 0;
	SysTimer_Init();

	SC1602_Init();
	SC1602_WriteStr( 0, "SimpleDriver" );
	SC1602_WriteStr( 1, " Test 0123456789" );


	LPC_IOCON->PIO0_7 = 0xD0;
	LPC_GPIO0->DIR |= 1<<7;
	LPC_GPIO0->DATA |= 1<<7;

    while(1)
    {
    	LPC_GPIO0->DATA ^= 1<<7;
    	for(i=0; i<1000000; i++);

    	SC1602_SetAddress( 1, 5 );
    	SC1602_WriteChar( (j++ & 2)? '-' : '|' );
    }
    return 0 ;
}
