
// MODULE:
//  AQM0802A-RN-GBW

#include "sc1602.h"
#include "i2c.h"

void SC1602_WaitUs(uint32_t t);
void SC1602_WaitMs(uint32_t t);

#define SC1602_I2C_ADDRESS					(0x7C)

#define SC1602_CMD_CLEAR_DISPLAY			(0x01)
#define SC1602_CMD_CURSOR_AT_HOME			(0x02)
#define SC1602_CMD_ENTRY_MODE_SET			(0x04)
#define SC1602_CMD_DISPLAY_ONOFF_CONTROL	(0x08)
#define SC1602_CMD_CURSOR_DISPLAY_SHIFT		(0x10)
#define SC1602_CMD_FUNCTION_SET				(0x20)
#define SC1602_CMD_CGRAM_ADDRESS_SET		(0x40)
#define SC1602_CMD_DDRAM_ADDRESS_SET		(0x80)

static void SC1602_I2C_Send( uint8_t adr, const uint8_t* data, uint8_t len )
{
	I2CClearBuffer();
	I2CWrite( &adr, 1 );
	I2CWrite( data, len );

	I2CEngine();
}

static void SC1602_I2C_WriteControl( uint8_t data )
{
	uint8_t ary[2];
	ary[0] = 0;
	ary[1] = data;
	SC1602_I2C_Send( SC1602_I2C_ADDRESS, ary, 2 );
}

void SC1602_Init( void )
{
	I2CInit();

	SC1602_WaitMs(50);

	SC1602_I2C_WriteControl(0x38);
	SC1602_WaitUs(50);

	SC1602_I2C_WriteControl(0x39);
	SC1602_WaitUs(50);

	SC1602_I2C_WriteControl(0x14);
	SC1602_WaitUs(50);

	SC1602_I2C_WriteControl(0x70);
	SC1602_WaitUs(50);

	SC1602_I2C_WriteControl(0x56);
	SC1602_WaitUs(50);

	SC1602_I2C_WriteControl(0x6C);
	SC1602_WaitMs(250);

	SC1602_I2C_WriteControl(0x38);
	SC1602_WaitUs(50);

	SC1602_I2C_WriteControl(0x0C);
	SC1602_WaitUs(50);

	SC1602_I2C_WriteControl(0x01);
	SC1602_WaitMs(2);
}

void SC1602_WriteChar( char data )
{
	uint8_t buffer[2];
	buffer[0] = 0xC0;
	buffer[1] = data;
	SC1602_I2C_Send( SC1602_I2C_ADDRESS, buffer, 2 );
	SC1602_WaitUs(40);
}

void SC1602_WriteData( uint8_t line, const uint8_t* data, uint32_t size )
{
	uint8_t i, eos;
	uint8_t buffer[SC1602_MAX_CHAR * 2];

	SC1602_SetAddress( line, 0 );

	eos = 0;
	for (i = 0; i < SC1602_MAX_CHAR; i++)
	{
		buffer[i * 2 + 0] = 0xC0;

		if (i >= size || data[i] <= 0x05)
		{
			eos = 1;
		}
		if (eos)
		{
			buffer[i * 2 + 1] = ' ';
		}
		else
		{
			buffer[i * 2 + 1] = data[i];
		}
	}
	SC1602_I2C_Send( SC1602_I2C_ADDRESS, buffer, i * 2 );
	SC1602_WaitUs(40);
}

void SC1602_WriteStr( uint8_t line, const char* data )
{
	uint32_t i;

	for(i=0; i<SC1602_MAX_CHAR; i++)
	{
		if(data[i] <= 5)
		{
			break;
		}
	}

	SC1602_WriteData( line, (const uint8_t*)data, i );
}

void SC1602_SetAddress( uint8_t line, uint8_t column )
{
	SC1602_DDRAM_AddressSet( (line==0? 0x00: 0x40) | column );
}

void SC1602_ClearDisplay( void )
{
	SC1602_I2C_WriteControl(SC1602_CMD_CLEAR_DISPLAY);
	SC1602_WaitMs(2);
}
void SC1602_CursorAtHome( void )
{
	SC1602_I2C_WriteControl(SC1602_CMD_CURSOR_AT_HOME);
	SC1602_WaitMs(2);
}
void SC1602_EntryModeSet( uint8_t params )
{
	SC1602_I2C_WriteControl(SC1602_CMD_ENTRY_MODE_SET | params);
	SC1602_WaitUs(40);
}
void SC1602_DisplayOnOffControl( uint8_t params )
{
	SC1602_I2C_WriteControl(SC1602_CMD_DISPLAY_ONOFF_CONTROL | params);
	SC1602_WaitUs(40);
}
void SC1602_CursorDisplayShift( uint8_t params )
{
	SC1602_I2C_WriteControl(SC1602_CMD_CURSOR_DISPLAY_SHIFT | params);
	SC1602_WaitUs(40);
}
void SC1602_FunctionSet( uint8_t params )
{
	SC1602_I2C_WriteControl(SC1602_CMD_FUNCTION_SET | params);
	SC1602_WaitUs(40);
}
void SC1602_CGRAM_AddressSet( uint8_t cgram_address )
{
	SC1602_I2C_WriteControl(SC1602_CMD_CGRAM_ADDRESS_SET | cgram_address);
	SC1602_WaitUs(40);
}
void SC1602_DDRAM_AddressSet( uint8_t ddram_address )
{
	SC1602_I2C_WriteControl(SC1602_CMD_DDRAM_ADDRESS_SET | ddram_address);
	SC1602_WaitUs(40);
}
