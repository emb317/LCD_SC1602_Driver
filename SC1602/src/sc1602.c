
// MODULE:
//  SC1602_BBWB-XA-LB-G

#include "sc1602.h"

void SC1602_WaitUs(uint32_t t);
void SC1602_WaitMs(uint32_t t);
void SC1602_InitIO( void );
void SC1602_Write4Bit( uint8_t rs, uint8_t data );
void SC1602_Write8Bit( uint8_t rs, uint8_t data );

#define SC1602_CMD_CLEAR_DISPLAY			(0x01)
#define SC1602_CMD_CURSOR_AT_HOME			(0x02)
#define SC1602_CMD_ENTRY_MODE_SET			(0x04)
#define SC1602_CMD_DISPLAY_ONOFF_CONTROL	(0x08)
#define SC1602_CMD_CURSOR_DISPLAY_SHIFT		(0x10)
#define SC1602_CMD_FUNCTION_SET				(0x20)
#define SC1602_CMD_CGRAM_ADDRESS_SET		(0x40)
#define SC1602_CMD_DDRAM_ADDRESS_SET		(0x80)

void SC1602_Init( void )
{
	SC1602_InitIO();


	SC1602_WaitMs(50);
	
	SC1602_Write4Bit(0, 0x3);

	SC1602_WaitMs(5);

	SC1602_Write4Bit(0, 0x3);

	SC1602_WaitMs(10);

	SC1602_Write4Bit(0, 0x3);

	SC1602_WaitMs(5);

	SC1602_Write4Bit(0, 0x2);
	SC1602_WaitMs(1);

	SC1602_FunctionSet(
		SC1602_IF_DATA_LENGTH_4 |
		SC1602_LINE_NUM_2 |
		SC1602_FONT_5X10_DOTS);

	SC1602_DisplayOnOffControl(
		SC1602_DISPLAY_OFF |
		SC1602_DISPLAY_CURSOR_OFF |
		SC1602_CURSOR_BLINK_OFF);

	SC1602_ClearDisplay();

	SC1602_EntryModeSet(
		SC1602_CURSOR_DIR_INCREMENT |
		SC1602_DISPLAY_SHIFT_OFF);

	SC1602_DisplayOnOffControl(
		SC1602_DISPLAY_ON |
		SC1602_DISPLAY_CURSOR_OFF |
		SC1602_CURSOR_BLINK_OFF);
}

void SC1602_WriteChar( char data )
{
	SC1602_Write8Bit(1, data);
	SC1602_WaitUs(40);
}

void SC1602_WriteData( uint8_t line, const uint8_t* data, uint32_t size )
{
	uint8_t i;

	SC1602_SetAddress( line, 0 );

	for(i=0; i<size && i < SC1602_MAX_CHAR; i++)
	{
		SC1602_Write8Bit(1, data[i]);
		SC1602_WaitUs(40);
	}
}

void SC1602_WriteStr( uint8_t line, const char* data )
{
	uint8_t i;
	uint8_t eos = 0;

	SC1602_SetAddress( line, 0 );

	for(i=0; i<SC1602_MAX_CHAR; i++)
	{
		if(eos || data[i] <= 5)
		{
			eos = 1;
			SC1602_Write8Bit(1, ' ');
		}
		else
		{
			SC1602_Write8Bit(1, data[i]);
		}
		SC1602_WaitUs(40);
	}
}

void SC1602_SetAddress( uint8_t line, uint8_t column )
{
	SC1602_DDRAM_AddressSet( (line==0? 0x00: 0x40) | column );
}

void SC1602_ClearDisplay( void )
{
	SC1602_Write8Bit(0, SC1602_CMD_CLEAR_DISPLAY);
	SC1602_WaitMs(2);
}
void SC1602_CursorAtHome( void )
{
	SC1602_Write8Bit(0, SC1602_CMD_CURSOR_AT_HOME);
	SC1602_WaitMs(2);
}
void SC1602_EntryModeSet( uint8_t params )
{
	SC1602_Write8Bit(0, SC1602_CMD_ENTRY_MODE_SET | params);
	SC1602_WaitUs(40);
}
void SC1602_DisplayOnOffControl( uint8_t params )
{
	SC1602_Write8Bit(0, SC1602_CMD_DISPLAY_ONOFF_CONTROL | params);
	SC1602_WaitUs(40);
}
void SC1602_CursorDisplayShift( uint8_t params )
{
	SC1602_Write8Bit(0, SC1602_CMD_CURSOR_DISPLAY_SHIFT | params);
	SC1602_WaitUs(40);
}
void SC1602_FunctionSet( uint8_t params )
{
	SC1602_Write8Bit(0, SC1602_CMD_FUNCTION_SET | params);
	SC1602_WaitUs(40);
}
void SC1602_CGRAM_AddressSet( uint8_t cgram_address )
{
	SC1602_Write8Bit(0, SC1602_CMD_CGRAM_ADDRESS_SET | cgram_address);
	SC1602_WaitUs(40);
}
void SC1602_DDRAM_AddressSet( uint8_t ddram_address )
{
	SC1602_Write8Bit(0, SC1602_CMD_DDRAM_ADDRESS_SET | ddram_address);
	SC1602_WaitUs(40);
}
