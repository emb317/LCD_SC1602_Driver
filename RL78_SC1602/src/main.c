/***********************************************************************/
/*                                                                     */
/*  FILE        :Main.c                                                */
/*  DATE        :                                                      */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/

#include "iodefine.h"

#include "sc1602.h"

void main(void);

void SC1602_WaitUs(uint8_t t);
void SC1602_WaitMs(uint8_t t);

void main(void)
{
	uint8_t j = 0;
	
	SC1602_Init();
	SC1602_WriteStr( 0, "SimpleDriver" );
	SC1602_WriteStr( 1, " Test 0123456789" );
	
	PU7_bit.no7 = 1;
	PM7_bit.no7 = 0;
	P7_bit.no7 = 0;
	while(1)
	{
		SC1602_WaitMs(250);

		P7 ^= 0x80;

    	SC1602_SetAddress( 1, 5 );
    	SC1602_WriteChar( (j++ & 2)? '-' : '|' );

		WDTE = 0xAC;
	}
}
