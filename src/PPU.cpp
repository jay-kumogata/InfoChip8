#include "InfoChip8.h"
#include "CPU.h"
#include "PPU.h"

/*-------------------------------------------------------------------*/
/*  PPU Resources                                                    */
/*-------------------------------------------------------------------*/

/* Registers */
WORD VRAM[CHIP8_WIDTH * CHIP8_HEIGHT];

/* Hexadecimal Fonts */
BYTE HEXFONT[ 0x50 ] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
  0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */
  0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
  0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 3 */
  0x90, 0x90, 0xF0, 0x10, 0x10, /* 4 */
  0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 5 */
  0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 6 */
  0xF0, 0x10, 0x20, 0x40, 0x40, /* 7 */
  0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 8 */
  0xF0, 0x90, 0xF0, 0x10, 0xF0, /* 9 */
  0xF0, 0x90, 0xF0, 0x90, 0x90, /* A */
  0xE0, 0x90, 0xE0, 0x90, 0xE0, /* B */
  0xF0, 0x80, 0x80, 0x80, 0xF0, /* C */
  0xE0, 0x90, 0x90, 0x90, 0xE0, /* D */
  0xF0, 0x80, 0xF0, 0x80, 0xF0, /* E */
  0xF0, 0x80, 0xF0, 0x80, 0x80  /* F */
};

/* Palette data */
WORD Chip8Palette[ 8 ] =
{
  0x7c00, 0x7de0, 0x7fe0, 0x03e0, 0x03ef, 0x03ff, 0x001f, 0x7c1f, 
};

/*-------------------------------------------------------------------*/
/*  PPU Functions                                                    */
/*-------------------------------------------------------------------*/

/* PPU Initialize */
int PPU_Init( void )
{
  PPU_Erase();
  return 0;
}

/* PPU Finalialize */
int PPU_Fin( void )
{
  return 0;
}

/* Draw Sprite */
void PPU_Draw( BYTE vx, BYTE vy, BYTE n, WORD i )
{
  int x, y;

  /* Clear Collision Detect Flag */
  VF = 0x00;

  for ( y = 0; y < n; y++ ) {
    for ( x = 0; x < 8; x++ ) {
      if ( CPU_Read( i + y ) & ( 0x80 >> x ) ) {
	 /* Set Collision Detect Flag */
	 if ( PPU_GetPixel( vx + x, vy + y ) )
	   VF = 0x01;
	 /* XOR Mode */
#if 0
	 PPU_XorPixel( vx + x, vy + y, 0x7c00 );
#else
	 PPU_XorPixel( vx + x, vy + y, Chip8Palette[ ( ( vy + y ) % 32 ) / 4 ] );
#endif
      }
    }
  }
}

