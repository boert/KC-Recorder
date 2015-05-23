#include "main.h"
#include "RLE.h"
#include "SD_CARD/dos.h"

// local prototypes
uint32_t RLEwaitEdge(void);
void	 RLEnewFileName(char *filename);
void 	 RLEdispFileName(char *filename);
void 	createCSWheader(uint32_t nEdges);


void RLErecord(void)
{
uint32_t x;
uint32_t nEdges = 0;	// number of signal level changes
uint8_t  i;
	eraseRAM();			// empty RAM header
	RAMcounter = 52;	// reserve 52 Bytes lengt for CSW header

	sermem_reset();		// reset sermem usage counter
	for (i=0;i<52;i++) sermem_readByte();	// skip 52 bytes of CSW header area

	RLEnewFileName(filename);	// generate a new nnnn.csw file name
	dispFileName(filename);	// and display it
	REC_LED_ON;
	cli();
	rleCounter = 0;
	sei();
	CTC_INT_ON;			// activate CTC interrupt

	do {						// main recording loop
		x = RLEwaitEdge();
		nEdges++;				// one more signal level change
		if (x <= 255) {			// write a byte
			sermem_writeByte(x);
			RAMcounter++;
		}
		else {					// write a long
			sermem_writeByte(0);	// 0 initializes a long
			for (i=0;i<4;i++) {
				sermem_writeByte((uint8_t) x);
				x = x >> 8;
			}
			RAMcounter += 5;	// zero byte + long var
		}

	} while (breakFlag == 0);	// end of main recording loop
	
	// finally switch REC LED off, deactivate CTC interrupt and return to input mode
	REC_LED_OFF;
	CTC_INT_OFF;
	createCSWheader(nEdges); // generate the CSW header in the serial memory
	writeFile_SD(filename);	// and write it to SD card
	pending = 0;		// allow next key input action
}	// end of RLErecord


void RLEreplay(void)
{
uint32_t	i;
uint8_t		byte;
uint32_t 	j;
uint32_t	x;
uint32_t	nEdges = 0;		// number of signal level changes
	sermem_reset();			// reset serial mem to position zero
//
	if (RAMcounter == 0) {
		pending = 0;		// allow next input
		return;				// and exit from replay
	}
//
	for (i=0;i<0x1D;i++) sermem_readByte();	// skip first part of CSW header
	for (i=0;i<4;i++) {		// read nEdges LittleEndian wise
		nEdges >>= 8;
		nEdges |= (uint32_t)sermem_readByte() << 24;
	}
	for (i=0;i<19;i++) sermem_readByte();	// skip remainder of CSW header
	
	if (nEdges > 0) {	
		PLAY_LED_ON;
		CTC_INT_ON;			// activate CTC interrupt
		for (i=0;i<nEdges;i++) {	// replay the RLE data
			byte = sermem_readByte(); 				
			if (byte) {
				downCounter = byte;
				while (downCounter) { };	// same timing mechanism as with record
				PORTA ^= 1<<REPLAY;
			}
			else {	// a very long pause
				x = 0;
				for (j=0;j<4;j++) {
					x >>= 8;
					x |= (uint32_t) sermem_readByte() << 24;
				}
				for (j=0;j<x;j++) {
					_delay_us(13.8);
					if (breakFlag) break;
				}
				PORTA ^= 1<<REPLAY;
			}
			if (breakFlag) break;
		}
		CTC_INT_OFF;	// deactivate CTC interrupt
		PLAY_LED_OFF;
	} // end of RLE replay
	pending = 0;
}	// end of RLEreplay

// ************* Local Functions **************

// generate the next free file name and deposit it in string "filename"
void RLEnewFileName(char *filename)
{
uint16_t	freeNum;
				//   12345678.
	strcpy(filename,"        .csw");	// initialize pattern for 8+3 file name
	freeNum = numFiles;
	do {
		freeNum++;
		Bin2Hex(filename,freeNum,4);		// next potential file number
	} while (FindName(filename) == FULL_MATCH);
	numFiles++;							// increment number of files
}

// wait for level change of REC signal
uint32_t RLEwaitEdge(void)
{
uint8_t level;
uint32_t	ticks;
	level = REC;
	do
	{
	} while ((level == REC) && (breakFlag == 0));	// wait for level change or end of recording

	cli();						// no interrups
	ticks = rleCounter;			// get counter value
	rleCounter = 0UL;			// reset counter for next time
	sei();						// allow interrupts again

	PORTA ^= 1<<REPLAY;					// reflect incoming Edge through beeper
	return ticks;
}	// end of RLEwaitEdge

// create the CSW header
void createCSWheader(uint32_t nEdges)
{
const char text1[] = "Compressed Square Wave";
const char text2[] = "KC-Recorder    ";
const uint16_t SampleRate = 36000u;
uint8_t i;
	sermem_reset();
	for (i=0;i<22;i++) sermem_writeByte(text1[i]);	// write csw header string
	sermem_writeByte(0x1A);							// terminator code
	sermem_writeByte(2);							// major version
	sermem_writeByte(0);							// minor version
	for (i=0;i<4;i++) sermem_writeByte(SampleRate >> (8*i));
	for (i=0;i<4;i++) sermem_writeByte(nEdges >> (8*i));
	sermem_writeByte(0x01);							// ordinary RLE compression
	sermem_writeByte(0x00);							// initial polarity LOW ???
	sermem_writeByte(0);							// no header extention
	for (i=0;i<16;i++) sermem_writeByte(text2[i]);	// creating software info
}
