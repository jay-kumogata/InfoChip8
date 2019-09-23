#include "InfoChip8.h"
#include "System.h"
#include "CPU.h"
#include "PPU.h"
#include "IO.h"
#include "APU.h"

/* RAM */
BYTE *RAM; 

/*-------------------------------------------------------------------*/
/*  Emulation Main Routine                                           */
/*-------------------------------------------------------------------*/
int InfoChip8_Main( void )
{
  /* Initialize */
  CPU_Init();
  PPU_Init();
  IO_Init();
  APU_Init();
  
  if ( InfoChip8_LoadRom( szRomName ) < 0 )
    return -1;

  /* Execute */
  while ( 1 ) {
  // for ( int i = 0; i < 20000; i++ ) { 
    CPU_Step( 10 );

    /* VSYNC occurs per 1/60 second */
    if ( Delay ) Delay--;
    if ( Sound ) {
      APU_BeepOn();
      Sound--;
    } else {
      APU_BeepOff();
    }

    InfoChip8_LoadFrame();
    InfoChip8_KeyState();
    InfoChip8_Wait( 20000 );

    /* Esc Check */
    if ( Esc ) break;
  }

  /* Finalize */
  APU_Fin();
  PPU_Fin();
  IO_Fin();
  CPU_Fin();

  return 0;
}
