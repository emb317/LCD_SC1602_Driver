
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

enum
{
	SC1602_PORT_E,
	SC1602_PORT_RS,
	SC1602_PORT_DB4,
	SC1602_PORT_DB5,
	SC1602_PORT_DB6,
	SC1602_PORT_DB7,
	SC1602_PORT_NUM
};

typedef struct SC1602_PORT
{
	volatile uint32_t* incon;
	volatile uint32_t* dir;
	volatile uint32_t* data;
	uint8_t   incon_value;
	uint8_t   bit;
	uint8_t   reserve[2];
}SC1602_PORT;

const static SC1602_PORT SC1602[SC1602_PORT_NUM]=
{
	{ &LPC_IOCON->PIO2_3,  &LPC_GPIO2->DIR, &LPC_GPIO2->DATA, 0xD0,  3, {0,0} }, // E	PIO2_3
	{ &LPC_IOCON->PIO2_6,  &LPC_GPIO2->DIR, &LPC_GPIO2->DATA, 0xD0,  6, {0,0} }, // RS	PIO2_6
	{ &LPC_IOCON->PIO2_7,  &LPC_GPIO2->DIR, &LPC_GPIO2->DATA, 0xD0,  7, {0,0} }, // DB4	PIO2_7
	{ &LPC_IOCON->PIO2_8,  &LPC_GPIO2->DIR, &LPC_GPIO2->DATA, 0xD0,  8, {0,0} }, // DB5	PIO2_8
	{ &LPC_IOCON->PIO2_9,  &LPC_GPIO2->DIR, &LPC_GPIO2->DATA, 0xD0,  9, {0,0} }, // DB6	PIO2_9
	{ &LPC_IOCON->PIO2_10, &LPC_GPIO2->DIR, &LPC_GPIO2->DATA, 0xD0, 10, {0,0} }  // DB7	PIO2_10
};

#define SC1602_H(port) (*SC1602[(port)].data  |= 1 << SC1602[(port)].bit)
#define SC1602_L(port) (*SC1602[(port)].data  &= ~(1 << SC1602[(port)].bit))

void SC1602_InitIO( void )
{
	uint8_t i;
	for(i=0; i<SC1602_PORT_NUM; i++)
	{
		*SC1602[i].incon  = SC1602[i].incon_value;
		*SC1602[i].dir   |= 1 << SC1602[i].bit;
		*SC1602[i].data  &= ~(1 << SC1602[i].bit);
	}
}

void SC1602_Write4Bit( uint8_t rs, uint8_t data )
{
	rs ? SC1602_H( SC1602_PORT_RS ): SC1602_L( SC1602_PORT_RS );
	data & (1<<0) ? SC1602_H( SC1602_PORT_DB4 ): SC1602_L( SC1602_PORT_DB4 );
	data & (1<<1) ? SC1602_H( SC1602_PORT_DB5 ): SC1602_L( SC1602_PORT_DB5 );
	data & (1<<2) ? SC1602_H( SC1602_PORT_DB6 ): SC1602_L( SC1602_PORT_DB6 );
	data & (1<<3) ? SC1602_H( SC1602_PORT_DB7 ): SC1602_L( SC1602_PORT_DB7 );

	SC1602_H( SC1602_PORT_E ); // E_High
	SC1602_WaitUs(1);
	SC1602_L( SC1602_PORT_E ); // E_Low
	SC1602_WaitUs(1);
}

void SC1602_Write8Bit( uint8_t rs, uint8_t data )
{
	SC1602_Write4Bit(rs, (data>>4) & 0x0F);
	SC1602_Write4Bit(rs,  data     & 0x0F);
	SC1602_WaitMs(1);
}
