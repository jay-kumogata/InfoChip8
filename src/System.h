#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include "Types.h"

/*-------------------------------------------------------------------*/
/*  ROM image file information                                       */
/*-------------------------------------------------------------------*/
extern char szRomName[ 256 ];

/*-------------------------------------------------------------------*/
/*  Function Prototype                                               */
/*-------------------------------------------------------------------*/
int InfoChip8_LoadRom( const char *pszFileName );
void InfoChip8_LoadFrame( void );
void InfoChip8_MessageBox( char *pszMsg, ... );
void InfoChip8_KeyState( void );
int InfoChip8_Rand( void );
void InfoChip8_Wait( int n );

int InfoChip8_SoundOpen( int samples_per_sync, int sample_rate );
void InfoChip8_SoundClose( void ); 
void InfoChip8_SoundOutput( int samples, BYTE *wave );

#endif /* !SYSTEM_H_INCLUDED */
