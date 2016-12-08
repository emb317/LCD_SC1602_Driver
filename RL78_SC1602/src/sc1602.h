
#ifndef _SC1602_H_
#define _SC1602_H_

#include "type.h"

#define SC1602_MAX_CHAR	16

// EntryModeSet
#define SC1602_CURSOR_DIR_INCREMENT	(0x02)
#define SC1602_CURSOR_DIR_DECREMENT	(0x00)
#define SC1602_DISPLAY_SHIFT_ON		(0x01)
#define SC1602_DISPLAY_SHIFT_OFF	(0x00)

// DisplayOnOffControl
#define SC1602_DISPLAY_ON			(0x04)
#define SC1602_DISPLAY_OFF			(0x00)
#define SC1602_DISPLAY_CURSOR_ON	(0x02)
#define SC1602_DISPLAY_CURSOR_OFF	(0x00)
#define SC1602_CURSOR_BLINK_ON		(0x01)
#define SC1602_CURSOR_BLINK_OFF		(0x00)

// CursorDisplayShift
#define SC1602_SHIFT_DISPLAY		(0x08)
#define SC1602_SHIFT_CURSOR			(0x00)
#define SC1602_SHIFT_TO_RIGHT		(0x04)
#define SC1602_SHIFT_TO_LEFT		(0x00)

// FunctionSet
#define SC1602_IF_DATA_LENGTH_8		(0x10)
#define SC1602_IF_DATA_LENGTH_4		(0x00)
#define SC1602_LINE_NUM_1			(0x00)
#define SC1602_LINE_NUM_2			(0x80)
#define SC1602_FONT_5X10_DOTS		(0x40)
#define SC1602_FONT_5X7_DOTS		(0x00)

void SC1602_Init( void );
void SC1602_WriteChar( char data );
void SC1602_WriteData( uint8_t line, const uint8_t* data, uint8_t size );
void SC1602_WriteStr( uint8_t line, const char* data );
void SC1602_SetAddress( uint8_t line, uint8_t column );

void SC1602_ClearDisplay( void );
void SC1602_CursorAtHome( void );
void SC1602_EntryModeSet( uint8_t params );
void SC1602_DisplayOnOffControl( uint8_t params );
void SC1602_CursorDisplayShift( uint8_t params );
void SC1602_FunctionSet( uint8_t params );
void SC1602_CGRAM_AddressSet( uint8_t cgram_address );
void SC1602_DDRAM_AddressSet( uint8_t ddram_address );

#endif // _SC1602_H_
