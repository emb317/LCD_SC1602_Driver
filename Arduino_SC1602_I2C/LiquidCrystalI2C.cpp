
#include "LiquidCrystalI2C.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

LiquidCrystalI2C::LiquidCrystalI2C()
{
}

void LiquidCrystalI2C::begin(uint8_t cols, uint8_t lines, uint8_t dotsize)
{
    Wire.begin();
    
    _displayfunction = LCD_8BITMODE | LCD_1LINE | dotsize | 0;
    if (lines > 1)
    {
        _displayfunction |= LCD_2LINE;
    }
    
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    _numlines = lines;

    setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);
    
    delay(50);

    command(LCD_FUNCTIONSET | _displayfunction);  // Function set

    command(LCD_FUNCTIONSET | _displayfunction | 0x01);  // Function set

    command(0x14);  // Internal OSC frequency

    command(0x70);  // Contrast set

    command(0x56);  // Power / ICON / Contrast control

    command(0x6C);  // Follower control
    delay(250);

    command(LCD_FUNCTIONSET | _displayfunction);  // Function set

    command(LCD_DISPLAYCONTROL | _displaycontrol);  // Display ON/OFF control

    clear();
    
    command(LCD_ENTRYMODESET | _displaymode);
}

void LiquidCrystalI2C::setRowOffsets(int row0, int row1, int row2, int row3)
{
    _row_offsets[0] = row0;
    _row_offsets[1] = row1;
    _row_offsets[2] = row2;
    _row_offsets[3] = row3;
}

/********** high level commands, for the user! */
void LiquidCrystalI2C::clear()
{
    command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
    delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidCrystalI2C::home()
{
    command(LCD_RETURNHOME);  // set cursor position to zero
    delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidCrystalI2C::setCursor(uint8_t col, uint8_t row)
{
    const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
    if ( row >= max_lines )
    {
        row = max_lines - 1;    // we count rows starting w/0
    }
    if ( row >= _numlines )
    {
        row = _numlines - 1;    // we count rows starting w/0
    }
    
    command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

// Turn the display on/off (quickly)
void LiquidCrystalI2C::noDisplay()
{
    _displaycontrol &= ~LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystalI2C::display()
{
    _displaycontrol |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LiquidCrystalI2C::noCursor()
{
    _displaycontrol &= ~LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystalI2C::cursor()
{
    _displaycontrol |= LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LiquidCrystalI2C::noBlink()
{
    _displaycontrol &= ~LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystalI2C::blink()
{
    _displaycontrol |= LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LiquidCrystalI2C::scrollDisplayLeft(void)
{
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidCrystalI2C::scrollDisplayRight(void)
{
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LiquidCrystalI2C::leftToRight(void)
{
    _displaymode |= LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LiquidCrystalI2C::rightToLeft(void)
{
    _displaymode &= ~LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LiquidCrystalI2C::autoscroll(void)
{
    _displaymode |= LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LiquidCrystalI2C::noAutoscroll(void)
{
    _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations with custom characters
void LiquidCrystalI2C::createChar(uint8_t location, uint8_t charmap[])
{
    location &= 0x7; // we only have 8 locations 0-7
    command(LCD_SETCGRAMADDR | (location << 3));
    for (int i=0; i<8; i++)
    {
        write(charmap[i]);
    }
}

/*********** mid level commands, for sending data/cmds */

inline void LiquidCrystalI2C::command(uint8_t value)
{
    send(value, LOW);
}

inline size_t LiquidCrystalI2C::write(uint8_t value)
{
    send(value, HIGH);
    return 1; // assume sucess
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void LiquidCrystalI2C::send(uint8_t value, uint8_t mode)
{
    Wire.beginTransmission(0x7C>>1);
    if( mode == LOW )
    {
        Wire.write( 0x00 );
    }
    else
    {
        Wire.write( 0xC0 );
    }
    Wire.write( value );
    Wire.endTransmission();
    delayMicroseconds(50);
}

