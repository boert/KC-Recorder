// Ansteuerung eines HD44780 kompatiblen LCD im 4-Bit-Interfacemodus
// http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/LCD-Ansteuerung
//
 
#ifndef LCD_ROUTINES_H
#define LCD_ROUTINES_H
 
#include <avr/pgmspace.h>
 
////////////////////////////////////////////////////////////////////////////////
// Pinbelegung f�r das LCD, an verwendete Pins anpassen
// alle Pins m�ssen in einem Port liegen
// die 4 Bit f�r die Daten m�ssen zusammenliegen, k�nnen aber an einer
// beliebigen Position anfangen  
 
#ifndef LCD_PORTS
#define LCD_PORTS
 
#define DATA_PORT	PORTB
#define DATA_DDR	DDRB
#define CTRL_PORT	PORTD
#define CTRL_DDR	DDRD
 
// 4 Bit LCD Datenbus DB4-DB7, das unterste Bit DB4 kann auf den Portbits 0..4 liegen
 
//  LCD DB4-DB7 <-->  PORTC Bit PC0-PC3
#define LCD_DB        	PB0
 
// LCD Steuersignale RS und EN
 
#define LCD_RS        	PD2
#define LCD_EN        	PD3
 
#endif // LCD_PORTS
 
////////////////////////////////////////////////////////////////////////////////
// LCD Ausf�hrungszeiten (MS=Millisekunden, US=Mikrosekunden)
 
#ifndef LCD_TIMINGS
#define LCD_TIMINGS
 
#define LCD_BOOTUP_MS           20	// increased from orig. 15, M.B.
#define LCD_ENABLE_US           1
#define LCD_WRITEDATA_US        46
#define LCD_COMMAND_US          42
 
#define LCD_SOFT_RESET_MS1      5
#define LCD_SOFT_RESET_MS2      1
#define LCD_SOFT_RESET_MS3      1
#define LCD_SET_4BITMODE_MS     5
 
#define LCD_CLEAR_DISPLAY_MS    2
#define LCD_CURSOR_HOME_MS      2
 
#endif // LCD_TIMINGS
 
////////////////////////////////////////////////////////////////////////////////
// Zeilendefinitionen des verwendeten LCD
// die Eintr�ge hier sollten f�r ein LCD mit einer Zeilenl�nge von 16 Zeichen passen
// bei anderen Zeilenl�ngen m�ssen diese Eintr�ge angepasst werden
 
#define LCD_DDADR_LINE1         0x00
#define LCD_DDADR_LINE2         0x40
#define LCD_DDADR_LINE3         0x10
#define LCD_DDADR_LINE4         0x50
 
////////////////////////////////////////////////////////////////////////////////
// Initialisierung: muss ganz am Anfang des Programms aufgerufen werden.
void lcd_init( void );
 
////////////////////////////////////////////////////////////////////////////////
// LCD l�schen
void lcd_clear( void );
 
////////////////////////////////////////////////////////////////////////////////
// Cursor in die erste Zeile, erste Spalte (Position 0,0)
void lcd_home( void );
 
////////////////////////////////////////////////////////////////////////////////
// Cursor an eine beliebige Position 
void lcd_setcursor( uint8_t x, uint8_t y );
 
////////////////////////////////////////////////////////////////////////////////
// Ausgabe eines einzelnen Zeichens an der aktuellen Cursorposition 
void lcd_data( uint8_t data );
 
////////////////////////////////////////////////////////////////////////////////
// Ausgabe eines Strings an der aktuellen Cursorposition 
// String liegt im RAM
void lcd_string( const char *data );
 
////////////////////////////////////////////////////////////////////////////////
// Ausgabe eines Strings an einer bestimmten Cursorposition 
// String liegt im RAM
void lcd_string_xy( uint8_t x, uint8_t y, const char *data );
 
////////////////////////////////////////////////////////////////////////////////
// Ausgabe einer Zahl an der aktuellen Cursorposition 
// Zahl liegt im RAM
void lcd_number( uint8_t number, uint8_t len, uint8_t fill );
 
////////////////////////////////////////////////////////////////////////////////
// Ausgabe einer Zahl an einer bestimmten Cursorposition 
// Zahl liegt im RAM
void lcd_number_xy( uint8_t x, uint8_t y, uint8_t number, uint8_t len, uint8_t fill );
 
////////////////////////////////////////////////////////////////////////////////
// Ausgabe eines Strings an der aktuellen Cursorposition
// String liegt im Flash 
void lcd_string_P( PGM_P data );
 
////////////////////////////////////////////////////////////////////////////////
// Definition eines benutzerdefinierten Sonderzeichens.
// data muss auf ein Array mit den Zeilencodes des zu definierenden Zeichens
// zeigen, Daten liegen im RAM
void lcd_generatechar( uint8_t code, const uint8_t *data, uint8_t lines );
 
////////////////////////////////////////////////////////////////////////////////
// Definition eines benutzerdefinierten Sonderzeichens.
// data muss auf ein Array mit den Zeilencodes des zu definierenden Zeichens
// zeigen, Daten liegen im FLASH
void lcd_generatechar_P( uint8_t code, PGM_P data, uint8_t lines );
 
////////////////////////////////////////////////////////////////////////////////
// Ausgabe eines Kommandos an das LCD.
void lcd_command( uint8_t data );
 
////////////////////////////////////////////////////////////////////////////////
// LCD Befehle und Argumente.
// zur Verwendung in lcd_command
 
// Clear Display -------------- 0b00000001
#define LCD_CLEAR_DISPLAY       0x01
 
// Cursor Home ---------------- 0b0000001x
#define LCD_CURSOR_HOME         0x02
 
// Set Entry Mode ------------- 0b000001xx
#define LCD_SET_ENTRY           0x04
 
#define LCD_ENTRY_DECREASE      0x00
#define LCD_ENTRY_INCREASE      0x02
#define LCD_ENTRY_NOSHIFT       0x00
#define LCD_ENTRY_SHIFT         0x01
 
// Set Display ---------------- 0b00001xxx
#define LCD_SET_DISPLAY         0x08
 
#define LCD_DISPLAY_OFF         0x00
#define LCD_DISPLAY_ON          0x04
#define LCD_CURSOR_OFF          0x00
#define LCD_CURSOR_ON           0x02
#define LCD_BLINKING_OFF        0x00
#define LCD_BLINKING_ON         0x01
 
// Set Shift ------------------ 0b0001xxxx
#define LCD_SET_SHIFT           0x10
 
#define LCD_CURSOR_MOVE         0x00
#define LCD_DISPLAY_SHIFT       0x08
#define LCD_SHIFT_LEFT          0x00
#define LCD_SHIFT_RIGHT         0x01
 
// Set Function --------------- 0b001xxxxx
#define LCD_SET_FUNCTION        0x20
 
#define LCD_FUNCTION_4BIT       0x00
#define LCD_FUNCTION_8BIT       0x10
#define LCD_FUNCTION_1LINE      0x00
#define LCD_FUNCTION_2LINE      0x08
#define LCD_FUNCTION_5X7        0x00
#define LCD_FUNCTION_5X10       0x04
 
#define LCD_SOFT_RESET          0x30
 
// Set CG RAM Address --------- 0b01xxxxxx  (Character Generator RAM)
#define LCD_SET_CGADR           0x40
 
#define LCD_GC_CHAR0            0
#define LCD_GC_CHAR1            1
#define LCD_GC_CHAR2            2
#define LCD_GC_CHAR3            3
#define LCD_GC_CHAR4            4
#define LCD_GC_CHAR5            5
#define LCD_GC_CHAR6            6
#define LCD_GC_CHAR7            7
 
// Set DD RAM Address --------- 0b1xxxxxxx  (Display Data RAM)
#define LCD_SET_DDADR           0x80
 
 
#endif // LCD_ROUTINES_H
