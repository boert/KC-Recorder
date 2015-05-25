// Ansteuerung eines HD44780 kompatiblen LCD im 4-Bit-Interfacemodus
// http://www.mikrocontroller.net/articles/HD44780
// http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/LCD-Ansteuerung
//
// Die Pinbelegung ist über defines in lcd.h einstellbar
 
#include <avr/io.h>
#include "main.h"
#include "lcd.h"
#include <util/delay.h>
 
/////////////////////////////////////////////////////////////////////////////////
// Erzeugt einen Enable-Puls
 
static void lcd_enable( void ) {
    CTRL_PORT |= (1<<LCD_EN);            // Enable auf 1 setzen
    _delay_us( LCD_ENABLE_US );         // kurze Pause
    CTRL_PORT &= ~(1<<LCD_EN);           // Enable auf 0 setzen
}
 
////////////////////////////////////////////////////////////////////////////////
// Sendet eine 4-bit Ausgabeoperation an das LCD
static void lcd_out( uint8_t data ) {
    data &= 0xF0;                       // obere 4 Bit maskieren
    DATA_PORT &= ~(0xF0>>(4-LCD_DB));    // Maske löschen
    DATA_PORT |= (data>>(4-LCD_DB));     // Bits setzen
    lcd_enable();
}
 
////////////////////////////////////////////////////////////////////////////////
// Initialisierung: muss ganz am Anfang des Programms aufgerufen werden.
void lcd_init( void ) {
// verwendete Pins auf Ausgang schalten
uint8_t pins = (0x0F << LCD_DB);   // 4 Datenleitungen
    DATA_DDR |= pins;
    DATA_PORT &= ~pins;
    pins = (1<<LCD_RS) | (1<<LCD_EN);  // RS und Enable
    CTRL_DDR |= pins;
    CTRL_PORT &= ~pins;

    // warten auf die Bereitschaft des LCD
    _delay_ms( LCD_BOOTUP_MS );
 
    // Soft-Reset muss 3mal hintereinander gesendet werden zur Initialisierung
    lcd_out( LCD_SOFT_RESET );
    _delay_ms( LCD_SOFT_RESET_MS1 );
 
    lcd_enable();
    _delay_ms( LCD_SOFT_RESET_MS2 );
 
    lcd_enable();
    _delay_ms( LCD_SOFT_RESET_MS3 );
 
    // 4-bit Modus aktivieren 
    lcd_out( LCD_SET_FUNCTION |
             LCD_FUNCTION_4BIT );
    _delay_ms( LCD_SET_4BITMODE_MS );
 
    // 4-bit Modus / 2 Zeilen / 5x7
    lcd_command( LCD_SET_FUNCTION |
                 LCD_FUNCTION_4BIT |
                 LCD_FUNCTION_2LINE |
                 LCD_FUNCTION_5X7 );
 
    // Display ein / Cursor aus / Blinken aus
    lcd_command(LCD_SET_DISPLAY |
                LCD_DISPLAY_ON |
                LCD_CURSOR_OFF |
                LCD_BLINKING_OFF); 
 
    // Cursor inkrement / kein Scrollen
    lcd_command( LCD_SET_ENTRY |
                 LCD_ENTRY_INCREASE |
                 LCD_ENTRY_NOSHIFT );
 
    lcd_clear();
}
 
////////////////////////////////////////////////////////////////////////////////
// Sendet ein Datenbyte an das LCD
void lcd_data( uint8_t data ) {
    CTRL_PORT |= (1<<LCD_RS);    // RS auf 1 setzen
 
    lcd_out( data );            // zuerst die oberen, 
    lcd_out( data<<4 );         // dann die unteren 4 Bit senden
 
    _delay_us( LCD_WRITEDATA_US );
}
 
////////////////////////////////////////////////////////////////////////////////
// Sendet einen Befehl an das LCD
void lcd_command( uint8_t data ) {
    CTRL_PORT &= ~(1<<LCD_RS);    // RS auf 0 setzen
 
    lcd_out( data );             // zuerst die oberen, 
    lcd_out( data<<4);           // dann die unteren 4 Bit senden
 
    _delay_us(LCD_COMMAND_US );
}
 
////////////////////////////////////////////////////////////////////////////////
// Sendet den Befehl zur Löschung des Displays
void lcd_clear( void ) {
    lcd_command( LCD_CLEAR_DISPLAY );
    _delay_ms( LCD_CLEAR_DISPLAY_MS );
}
 
////////////////////////////////////////////////////////////////////////////////
// Sendet den Befehl: Cursor Home
void lcd_home( void ) {
    lcd_command( LCD_CURSOR_HOME );
    _delay_ms( LCD_CURSOR_HOME_MS );
}
 
////////////////////////////////////////////////////////////////////////////////
// Setzt den Cursor in Zeile y (0..3) Spalte x (0..15)
 
void lcd_setcursor( uint8_t x, uint8_t y ) {
    uint8_t data;
 
    switch (y) {
        case 0:    // 1. Zeile
            data = LCD_SET_DDADR + LCD_DDADR_LINE1;
            break;
        case 1:    // 2. Zeile
            data = LCD_SET_DDADR + LCD_DDADR_LINE2;
            break;
        case 2:    // 3. Zeile
            data = LCD_SET_DDADR + LCD_DDADR_LINE3;
            break;
        case 3:    // 4. Zeile
            data = LCD_SET_DDADR + LCD_DDADR_LINE4;
            break;
        default:    
            return; // für den Fall einer falschen Zeile
    }
    data += x;
 
    lcd_command( data );
}
 
////////////////////////////////////////////////////////////////////////////////
// Schreibt einen String auf das LCD
 
void lcd_string( const char *data ) {
    while( *data != '\0' )
        lcd_data( *data++ );
}
 
void lcd_string_xy( uint8_t x, uint8_t y, const char *data ) {
    lcd_setcursor( x, y );
    lcd_string( data );
}
 
////////////////////////////////////////////////////////////////////////////////
// Schreibt eine Zahl auf das LCD
 
void lcd_number( uint8_t number, uint8_t len, uint8_t fill ) {
    uint8_t digit1 = 0;
    uint8_t digit2 = 0;
    while (number >= 100) {
        digit1++;
        number -= 100;
    }
    while (number >= 10) {
        digit2++;
        number -= 10;
    }
    if (len > 2) lcd_data( (digit1 != 0) ? digit1+'0' : fill );
    if (len > 1) lcd_data( ((digit1 != 0) || (digit2 != 0)) ? digit2+'0' : fill );
    lcd_data( number+'0' );
}
 
void lcd_number_xy( uint8_t x, uint8_t y, uint8_t number, uint8_t len, uint8_t fill ) {
    lcd_setcursor( x, y );
    lcd_number( number, len, fill );
}
 

////////////////////////////////////////////////////////////////////////////////
// Schreibt eine 2stellige Hexzahl
void lcd_hexnumber( uint8_t number) 
{
    void lcd_nibble( uint8_t num)
    {
        if ( num < 10)
        {
            lcd_data( '0' + num);
        }
        else
        {
            lcd_data( 'A'  - 10 + num);
        }
    }

    lcd_nibble( number >> 4);
    lcd_nibble( number &  0xf);
}
 
////////////////////////////////////////////////////////////////////////////////
// Schreibt einen String auf das LCD
// String liegt direkt im Flash Speicher
 
void lcd_string_P( PGM_P data ) {
    uint8_t tmp;
 
    tmp = pgm_read_byte( data );
    while( tmp != '\0' ) {        
        lcd_data( tmp );
        data++;
        tmp = pgm_read_byte( data );
    }
}
 
////////////////////////////////////////////////////////////////////////////////
// Schreibt ein Zeichen in den Character Generator RAM
// Daten liegen direkt im RAM
 
void lcd_generatechar( uint8_t code, const uint8_t *data, uint8_t lines ) {
    uint8_t i;
 
    // Startposition des Zeichens einstellen
    lcd_command( LCD_SET_CGADR | (code<<3) );
    // Bitmuster übertragen
    for ( i=0; i<lines; i++ ) {
        lcd_data( *data++ );
    }
}
 
////////////////////////////////////////////////////////////////////////////////
// Schreibt ein Zeichen in den Character Generator RAM
// Daten liegen direkt im Flash-Speicher
 
void lcd_generatechar_P( uint8_t code, PGM_P data, uint8_t lines ) {
    uint8_t i;
 
    // Startposition des Zeichens einstellen
    lcd_command( LCD_SET_CGADR | (code<<3) );
    // Bitmuster übertragen
    for ( i=0; i<lines; i++ ) {
        lcd_data( pgm_read_byte(data) );
        data++;
    }
}


 
////////////////////////////////////////////////////////////////////////////////
// draw a logo (2x4) on the screen
// parameters: x & y position of left top
void lcd_put_logo( uint8_t x, uint8_t y)
{
    LCD_INIT_LOGO();
    lcd_setcursor( x, y + 0);
    lcd_data( 0);
    lcd_data( 1);
    lcd_data( 2);
    lcd_data( 3);

    lcd_setcursor( x, y + 1);
    lcd_data( 4);
    lcd_data( 5);
    lcd_data( 6);
    lcd_data( 7);
}


////////////////////////////////////////////////////////////////////////////////
// put a progress bar with some graphic chars on specific line
// length is given in 'pixel' (= char * 5)
void lcd_put_bar( uint8_t line, uint8_t length)
{
    uint8_t index;
    ldiv_t  value;

    LCD_INIT_BARS();
    lcd_setcursor( 0, line);
    value = ldiv( length, 5);

    // solid part
    for( index = 0; index < value.quot; index++)
    {
        lcd_data( 0xff );
    }

    // small part
    lcd_data( value.rem);
    index++;

    for( ; index < 16; index++)
    {
        lcd_data( ' ');
    }

}
