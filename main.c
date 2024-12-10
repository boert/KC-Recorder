#include "main.h"

uint8_t 			serbuffer[SERBUFSIZE];
uint16_t			serRdPointer;
volatile uint16_t	serInPointer = 0;
volatile int16_t	buf_usage = 0;
volatile uint32_t	rleCounter;
volatile uint8_t	downCounter;

ISR (USART0_RX_vect)	// serial Interface
{
uint8_t c;
	c = UDR0;			// get one more char from serial line
	serbuffer[serInPointer++] = c;
	serInPointer &= SERBUFMASK;
	buf_usage++;
}	// end of UART ISR


ISR (TIMER0_COMPA_vect)
{
	rleCounter++;	// 32 bit counter for recording
	downCounter--;	// 8  bit counter for replay
}
	
static FILE uart_output = FDEV_SETUP_STREAM(uart_putChar_stream, NULL, _FDEV_SETUP_WRITE);

int main(void)
{

// general settings for all ports
	DDRA = ALLPINSIN | (1<<PA4);	// only PLAY is output
	DDRB = ALLPINSIN;	// LCD ports will be initialized separately 
	DDRC = ALLPINSIN;	// port C is input
	DDRD = ALLPINSIN; 
	PORTA = PULLUP & ~((1<<PA2) | (1<<PA4));	// activate internal pull up's except REC, set PLAY to GND
	PORTB = PULLUP;		// activate internal pull up's
	PORTC = PULLUP;		// activate internal pull up's
	PORTD = PULLUP;		// activate internal pull up's

	initCounter();	// start counter but do not yet activate interrupt for it
	initUART();		// prepare UART settings
	lcd_init();		// initialize LCD, cursor off
	sermem_init();	// initialize serial memory
    ramcheck();     // check memory size
	// output redirect
	stdout = &uart_output;
	sei();
	monitor();	// monitor is the main loop
}


void initCounter(void)		// start Counter0 in CTC mode, but do not yet activate interrupt
{
	TCCR0A  = 1<<WGM01;		// CTC mode
	TCCR0B |= 1<<CS01;		// Prescaler 8
	OCR0A = CTCCONST - 1;	// set counter limit
}

void initUART(void)
{
	// Set baud rate
	UBRR0H = (uint8_t) (USART_UBRR_VALUE>>8);
	UBRR0L = (uint8_t) USART_UBRR_VALUE;
	// Enable receive interrupt, receiver and transmitter
	UCSR0B = (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);	// 8N1 format
}	// end of initUART

int uart_putChar_stream(char c, FILE* stream)
{
    return uart_putChar(c);
}

int uart_putChar(char c)
{
	while (!(UCSR0A & (1<<UDRE0)))  // wait until send allowed
	{ 
	}
	UDR0 = c;
	return 0;
}	// end of uart_putChar

uint8_t uart_getChar(void)
{
uint8_t	c;
	while (buf_usage <= 0)
	{ 
	}
	cli();
	c = serbuffer[serRdPointer++];
	serRdPointer &= SERBUFMASK;
	buf_usage--;
	sei();
	return c;
}	// end of uart_getChar


// perfom a memory check 
// return how many memory (in kBytes) is availible
uint16_t ramcheck( void)
{
    uint32_t index;
    uint32_t value;
    uint16_t max_kbytes = 512 * 4;


    lcd_string_P( PSTR( "RAM check"));
    lcd_setcursor( 0, 1);
    lcd_string_P( PSTR( "fill..."));

    for( index = 0; index < max_kbytes; index++)
    {
        sermem_writeDword( index);
        sermem_skip( 1024 - sizeof( index)); 
    }
    sermem_reset();

    lcd_setcursor( 0, 2);
    lcd_string_P( PSTR( "test..."));
    index = 0;
    while( 1)
    {
        value = sermem_readDword();
        sermem_skip( 1024 - sizeof( index)); 

        if( value != index) break;
        index++;
        if( index == max_kbytes) break;
    }
    sermem_reset();

    lcd_setcursor( 0, 3);
    lcd_string_P( PSTR( "found "));
    utoa( index, (char *) recMem, 10);
    lcd_string( (char *) recMem);
    lcd_string_P( PSTR( " kB"));

    _delay_ms( 2000);
    lcd_clear();

    return  (uint16_t) index;
}
