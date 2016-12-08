
#include "sc1602.h"

// RL78/G13 Stick
// R5F100LEAFA

#include "iodefine.h"

void SC1602_WaitUs(uint8_t t)
{
	uint8_t i;
	uint8_t n;
	for(i=0; i<7; i++)
	{
		n = t;
		while(n--){}
	}
}

void SC1602_WaitMs(uint8_t t)
{
	uint16_t i;
	while(t--)
	{
		for(i=0; i<6400-1; i++){}
	}
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
	volatile uint8_t* pu;
	volatile uint8_t* data;
	uint8_t   bit;
	uint8_t   reserve[1];
}SC1602_PORT;

const static SC1602_PORT SC1602[SC1602_PORT_NUM]=
{
	{ &PM5,  &P5, 4, {0} }, // E	P54
	{ &PM5,  &P5, 5, {0} }, // RS	P55
	{ &PM5,  &P5, 3, {0} }, // DB4	P53
	{ &PM5,  &P5, 2, {0} }, // DB5	P52
	{ &PM5,  &P5, 1, {0} }, // DB6	P51
	{ &PM5,  &P5, 0, {0} }  // DB7	P50
};

#define SC1602_H(port) (*SC1602[(port)].data  |= 1 << SC1602[(port)].bit)
#define SC1602_L(port) (*SC1602[(port)].data  &= ~(1 << SC1602[(port)].bit))

void SC1602_InitIO( void )
{
	uint8_t i;
	for(i=0; i<SC1602_PORT_NUM; i++)
	{
		*SC1602[i].pu    &= ~(1 << SC1602[i].bit);
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
