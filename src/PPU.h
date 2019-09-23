#define CHIP8_WIDTH  64
#define CHIP8_HEIGHT 32

#define CHIP8_FONT_TOP  0x0F10  

/* Registers */
extern WORD VRAM[CHIP8_WIDTH * CHIP8_HEIGHT];

/* Chip8 Fonts */
extern BYTE HEXFONT[ 0x50 ];

/*-------------------------------------------------------------------*/
/*  Function Prototype                                               */
/*-------------------------------------------------------------------*/
extern void PPU_Draw( BYTE vx, BYTE vy, BYTE n, WORD i );
extern int PPU_Init( void );
extern int PPU_Fin( void );

/*-------------------------------------------------------------------*/
/*  Macros                                                           */
/*-------------------------------------------------------------------*/
#define PPU_Erase() { \
  int x, y; \
  for ( x = 0; x < CHIP8_WIDTH; x++ ) { \
    for ( y = 0; y < CHIP8_HEIGHT; y++ ) { \
      PPU_SetPixel( x, y, 0 ); \
    } \
  } \
}

#define PPU_XorPixel( x, y, c ) { \
  VRAM[ ((y)%32) * CHIP8_WIDTH + ((x)%64) ] ^= c; \
}

#define PPU_SetPixel( x, y, c ) { \
  VRAM[ ((y)%32) * CHIP8_WIDTH + ((x)%64) ] = c; \
}

#define PPU_GetPixel( x, y ) \
  VRAM[ ((y)%32) * CHIP8_WIDTH + ((x)%64) ]
