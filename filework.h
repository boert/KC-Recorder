#ifndef FILEWORK_H
#define FILEWORK_H

#define MAXFILES	512	// max expected # files per dir
#define MAXDIRS		64	// max expected # of subdirs
#define DEL_KEY		PD6
#define LOAD_KEY	PD4
#define UP_KEY		PD7
#define DOWN_KEY	PD5
#define BACK_TO_REC	" ==> Menu 1"

#define YES_KEY		PD6
#define NO_KEY		PD4

enum    filetypes { type_tap, type_kcc, type_kcb, type_other};

extern volatile uint8_t   breakFlag; // this is only a declaration
extern enum filetypes     filetype;
extern          uint32_t  nblocks;
extern			uint8_t	  RLEmode;

void            FileDialog( void);
enum filetypes  getFileType( char* filename);

#endif // FILEWORK_H
