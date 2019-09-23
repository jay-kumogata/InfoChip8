#include "InfoChip8.h"
#include "System.h"
#include "CPU.h"
#include "PPU.h"
#include "IO.h"

/*-------------------------------------------------------------------*/
/*  I/O Resources                                                    */
/*-------------------------------------------------------------------*/

/* Delay Timer */
BYTE Delay;

/* Sound Timer */
BYTE Sound;

/* Key State : FEDCBA9876543210 */
WORD Key;
BYTE Esc;

/*-------------------------------------------------------------------*/
/*  I/O Functions                                                    */
/*-------------------------------------------------------------------*/

/* I/O Initialize */
int IO_Init( void )
{
  /* Delay Timer */
  Delay = 0;

  /* Sound Timer */
  Sound = 0;

  /* Key State : FEDCBA9876543210 */
  Key = 0;
  Esc = 0;

  return 0;
}

/* I/O Finalialize */
int IO_Fin( void )
{
  return 0;
}

