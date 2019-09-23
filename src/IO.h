#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include "Types.h"

/* Delay Timer */
extern BYTE Delay;

/* Sound Timer */
extern BYTE Sound;

/* Key State : FEDCBA9876543210 */
extern WORD Key;
extern BYTE Esc;

/*-------------------------------------------------------------------*/
/*  Function Prototype                                               */
/*-------------------------------------------------------------------*/

/* I/O Initialize */
int IO_Init( void );

/* I/O Finalialize */
int IO_Fin( void );

#endif /* !IO_H_INCLUDED */
