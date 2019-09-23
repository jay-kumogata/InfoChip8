#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "Types.h"

/*-------------------------------------------------------------------*/
/*  CPU Resources                                                    */
/*-------------------------------------------------------------------*/

/* Registers */
extern WORD PC;
extern WORD I;
extern BYTE V[ 0x10 ];
extern WORD INST;

/* Stack */
extern WORD STACK[ 0x10 ];
extern BYTE SP;

/*-------------------------------------------------------------------*/
/*  Function Prototype                                               */
/*-------------------------------------------------------------------*/
int CPU_Init( void );
int CPU_Fin( void );
int CPU_Step( int cycle );
BYTE CPU_Read( WORD wAddr );
WORD CPU_ReadW( WORD wAddr );
void CPU_Write( WORD wAddr, BYTE byData );

/*-------------------------------------------------------------------*/
/*  Macros                                                           */
/*-------------------------------------------------------------------*/
#define VX       V[ X ]
#define VY       V[ Y ]
#define VF       V[ 0xF ]
#define V0       V[ 0x0 ]

#define NNN      (INST & 0x0FFF)
#define KK       (INST & 0x00FF)
#define X        ( (INST & 0x0F00) >> 8 ) 
#define Y        ( (INST & 0x00F0) >> 4 ) 
#define N        (INST & 0x000F)

#define CPU_Bcd(a) { \
  CPU_Write( I    , (a) / 100 ); \
  CPU_Write( I + 1, ( (a) % 100 ) / 10 ); \
  CPU_Write( I + 2, (a) % 10 ); \
}

#define CPU_Save(a) { \
  BYTE bT0; \
  for ( bT0 = 0; bT0 <= (a); bT0++ ) { \
    CPU_Write( I + bT0, V[ bT0 ] ); \
  } \
}

#define CPU_Load(a) { \
  BYTE bT0; \
  for ( bT0 = 0; bT0 <= (a); bT0++ ) { \
    V[ bT0 ] = CPU_Read( I + bT0 ); \
  } \
}

#define CPU_Wait(a) { \
  while ( !Key ) { \
    InfoChip8_KeyState(); \
    InfoChip8_Wait( 1 ); \
  } \
  for ( int n = 0; n < 0x10; n++ ) { \
    if ( Key & ( 1 << n ) ) { \
      V[ (a) ] = n; \
      break; \
    } \
  } \
}

#endif /* !CPU_H_INCLUDED */
