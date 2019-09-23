#include "InfoChip8.h"
#include "System.h"
#include "CPU.h"
#include "PPU.h"
#include "IO.h"
#include "APU.h"

/*-------------------------------------------------------------------*/
/*  APU Resources                                                    */
/*-------------------------------------------------------------------*/

BYTE Beep[ SAMPLES ];
BYTE None[ SAMPLES ];

/*-------------------------------------------------------------------*/
/*  APU Functions                                                    */
/*-------------------------------------------------------------------*/

/* APU Initialize */
int APU_Init( void )
{
  int i;

  /* Sound Device Open */
  InfoChip8_SoundOpen( SAMPLES, SAMPLE_RATE );

  /* Beep : Recutangler Wave */
  for ( i = 0; i < SAMPLES; i++ ) {
    Beep[ i ] = ( ( i / 63 ) % 2 ) ? 0xFF : 0x00; 
  }

  /* None */
  for ( i = 0; i < SAMPLES; i++ ) {
    None[ i ] = 0x00;
  }
  return( 0 );
}

/* APU Finalialize */
int APU_Fin( void )
{
  /* Sound Device Close */
  InfoChip8_SoundClose(); 

  return 0;
}
