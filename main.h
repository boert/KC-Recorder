#ifndef MAIN_H_
#define MAIN_H_

#define VERSION		"1.0B 25-May-2015"
#define F_CPU 18432000ul
#define USART_BAUD 9600ul
#define USART_UBRR_VALUE ((F_CPU/(USART_BAUD<<4))-1)
#define SERBUFSIZE 1024		// must be a 2^n number
#define SERBUFMASK (SERBUFSIZE - 1)
#define ALLPINSOUT	0b11111111	// entire Atmel port to Output
#define ALLPINSIN	0b00000000	// entire Atmel port to Input
#define PULLUP		0b11111111
#define CRLF		"\r\n"
#define BYTES_LINE	16			// bytes per line for RAM dump
#define ERR 		-1
#define MEMSIZE		160			// Ram Buffer for direct access to file header
#define REPLAY		PA4			// replay is pin PA4
#define AUX			(PINA & (1<<PA3))
#define REC			(PINA & (1<<PA2))
#define KEYMASK		0xF0		// mask for key input pins
#define CTCCONST	32			// limit for counter cycle
#define CTC_INT_ON	TIMSK0 |= (1<<OCIE0A)	// switch on CTC interrupt
#define CTC_INT_OFF	TIMSK0 &= ~(1<<OCIE0A)	// switch off CTC interrupt
// LED control actions
#define REC_LED_ON	DDRD |= 1<<PD6; PORTD &= ~(1<<PD6)
#define REC_LED_OFF	DDRD &= ~(1<<PD6); PORTD |= 1<<PD6
#define PLAY_LED_ON	DDRD |= 1<<PD4; PORTD &= ~(1<<PD4)
#define PLAY_LED_OFF DDRD &= ~(1<<PD4); PORTD |= 1<<PD4
// keys for primary menu:
#define REC_KEY		PD6
#define PLAY_KEY	PD4
#define BREAK_KEY	PD5
#define DIR_KEY		PD7
#define TRUE		0xFF
#define FALSE		0


// used libraries
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lcd.h"
#include "sermem.h"

// function prototypes
void 	initCounter(void);
void 	initUART(void);
int     uart_putChar(uint8_t c);
uint8_t	uart_getChar(void);
void 	monitor(void);
void 	dispFileName(char *filename);
void 	eraseRAM(void);
void 	writeFile_SD(char *filename);
void 	Bin2Hex(char *here,uint16_t number,uint8_t digits);
uint16_t ramcheck( void);


// variable declarations
extern 			uint8_t 	serbuffer[];	// buffer for ser. communication
extern volatile int16_t		buf_usage;
extern volatile uint16_t 	serInPointer;
extern volatile uint32_t	rleCounter;		// 32 bit counter for recording
extern volatile uint8_t		downCounter;	// 8  bit counter for replay
extern			uint32_t	RAMcounter;		// counter for current record memory position
extern			uint8_t		recMem[];	
extern			uint8_t		RLEmode;
extern 			uint16_t 	numFiles;	
extern			char		filename[];

#endif	// MAIN_H_

