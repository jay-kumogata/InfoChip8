#include <stdio.h>

#include "InfoChip8.h"
#include "System.h"
#include "CPU.h"
#include "PPU.h"
#include "IO.h"

/*-------------------------------------------------------------------*/
/*  CPU Resources                                                    */
/*-------------------------------------------------------------------*/

/* Registers */
WORD PC;
WORD I;
BYTE V[ 0x10 ];
WORD INST;

/* Stack */
WORD STACK[ 0x10 ];
BYTE SP;

/*-------------------------------------------------------------------*/
/*  CPU Emulation                                                    */
/*-------------------------------------------------------------------*/

/* Initialize */
int CPU_Init( void )
{
  PC = 0x0200;
  SP = 0xF;

  return 0;
}

/* Finaliaze */
int CPU_Fin( void )
{
  return 0;
}

int CPU_Step( int cycle )
{
  while ( cycle-- ) {
    INST = CPU_ReadW( PC ); PC += 2;

    switch ( INST & 0xF000 ) {
    case 0x0000:
      /* 0x00E0 : Erase the Screen */
      if ( INST == 0x00E0 ) {
	 PPU_Erase(); 
      } 
      /* 0x00EE : Return from a CHIP-8 subroutine */
      else if ( INST == 0x00EE ) {
	SP++; 
	PC = STACK[ SP & 0xF ];
      }
      break;

    case 0x1000:
      /* 0x1NNN : Jump to NNN */
      PC = NNN;
      break;

    case 0x2000:
      /* 0x2NNN : Call CHIP-8 sub-routine at NNN */
      /*          ( 16 successive calls max)     */
      STACK[ SP & 0xF ] = PC; SP--;
      PC = NNN;
      break;

    case 0x3000:
      /* 0x3XKK : Skip next instruction if VX == KK */
      if ( VX == KK ) { PC += 2; }
      break;

    case 0x4000:
      /* 0x4XKK : Skip next instruction if VX != KK */
      if ( VX != KK ) { PC += 2; }
      break;
      
    case 0x5000:
      /* 0x5XY0 : Skip next instruction if VX == VY */
      if ( VX == VY ) { PC += 2; }
      break;

    case 0x6000:
      /* 0x6XKK : VX = KK */ 
      VX = KK;
      break;

    case 0x7000:
      /* 0x7XKK : VX = VX + KK */ 
      VX += KK;
      break;

    case 0x8000:
      switch ( INST & 0x000F ) {
      /* 0x8XY0 : VX = VY */ 
      case 0x0: 	
	 VX = VY;
	 break;

      /* 0x8XY1 : VX = VX OR VY */ 	 
      case 0x1:
	 VX |= VY;
	 break;

      /* 0x8XY2 : VX = VX AND VY */ 	 
      case 0x2:
	 VX &= VY;
	 break;
	 
      /* 0x8XY3 : VX = VX XOR VY */ 	 
      case 0x3:
	 VX ^= VY;
	 break;

      /* 0x8XY4 : VX = VX + VY, VF = Carry */ 	 
      case 0x4:
	 VF = ( (WORD)VX + VY ) & 0x100 ? 1 : 0;
        VX += VY;
	 break;

      /* 0x8XY5 : VX = VX - VY, VF = Not Borrow */
      case 0x5:
	 VF = ( VX < VY ) ? 0 : 1;
        VX -= VY;
	 break;

      /* 0x8XY6 : VX = VX SHR 1 ( VX = VX / 2 ), VF = Carry */
      case 0x6:
	 VF = VX & 0x01;
	 VX >>= 1;
	 break;

      /* 0x8XY7 : VX = VY - VX, VF = Not Borrow */
      case 0x7:
	 VF = ( VY < VX ) ? 0 : 1;
        VX = VY - VX;
	 break;

      /* 0x8XYE : VX = VX SHL 1 ( VX = VX * 2 ), VF = Carry */
      case 0xE:
	 VF = VX & 0x80 ? 1 : 0;
	 VX <<= 1;
	 break;

      /* Unknown Instruction */
      default:
	 break;
      }
      break;

    case 0x9000:
      /* 0x9XY0 : Skip next instruction if VX != VY */
      if ( VX != VY ) { PC += 2; }
      break;

    case 0xA000:
      /* 0xANNN : I = NNN */
      I = NNN;
      break;

    case 0xB000:
      /* 0xBNNN : Jump to NNN + V0 */
      PC = NNN + V0;
      break;

    case 0xC000:
      /* 0xCXKK : VX = Random number AND KK */
#if 1
      /* TETRIS */
      VX = InfoChip8_Rand() & KK;
#else
      /* VBRIX */
      VX = InfoChip8_Rand() % KK;
#endif
      break;

    case 0xD000:
      /* 0xDXYN : Draws a sprite (VX,VY) starting at M(I). */
      /*          VF = collision. */
      PPU_Draw( VX, VY, N, I );
      break;

    case 0xE000:
      switch ( INST & 0x00FF ) {
      /* 0xEX9E : Skip next instruction if key VX pressed  */
      case 0x9E:
	 if ( Key & ( 1 << VX ) ) {
	   PC += 2;
	 }
	 break;

      /* 0xEXA1 : Skip next instruction if key VX not pressed  */
      case 0xA1:
	 if ( !( Key & ( 1 << VX ) ) ) {
	   PC += 2;
	 }
	 break;
      }
      break;

    case 0xF000:
      switch ( INST & 0x00FF ) {
      /* 0xFX07 : VX = Delay timer */
      case 0x07:
	VX = Delay;
	break;

      /* 0xFX0A : Waits a keypress and stores it in VX */
      case 0x0A:
	CPU_Wait( X );
	//printf("WAIT:%d\n", VX );
	break;

      /* 0xFX15 : Delay timer = VX */
      case 0x15:
	Delay = VX;
	break;

      /* 0xFX18 : Sound timer = VX */
      case 0x18:
	Sound = VX;
	//printf("Sound:%d\n", VX );
	break;

      /* 0xFX1E : I = I + VX */
      case 0x1E:
	I += VX;
	break;

      /* 0xFX29 : I points to the 4 x 5 font sprite of hex char in VX */
      case 0x29:
	 I = CHIP8_FONT_TOP + VX * 5;
	 break;

      /* 0xFX33 : Store BCD representation of VX in M(I)..M(I+2) */	 
      case 0x33:
	 CPU_Bcd( VX );
	 //printf( "BCD:%d\n", VX );
	 break;

      /* 0xFX55 : Save V0..VX in memory starting at M(I) */
      case 0x55:
	 CPU_Save( X );
	 break;

      /* 0xFX65 : Load V0..VX from memory starting at M(I) */
      case 0x65:
	 CPU_Load( X );
	 break;
      }
      break;

    default:
      break;
    } 

    /* */
#if 0
    printf("PC:%04x,I:%04x,INST:%04x,SP:%04x\n",PC,I,INST,SP);
    printf("V0:%02x,V1:%02x,V2:%02x,V3:%02x\n",V[0],V[1],V[2],V[3]);
    printf("V4:%02x,V5:%02x,V6:%02x,V7:%02x\n",V[4],V[5],V[6],V[7]);
    printf("V8:%02x,V9:%02x,VA:%02x,VB:%02x\n",V[8],V[9],V[10],V[11]);
    printf("VC:%02x,VD:%02x,VE:%02x,VV:%02x\n\n",V[12],V[13],V[14],V[15]);
#endif
  }
  return 0;
}

/*-------------------------------------------------------------------*/
/*  Memory Emulation                                                 */
/*-------------------------------------------------------------------*/
BYTE CPU_Read( WORD wAddr )
{
  if ( wAddr < 0x200 ) {
    return 0;
  } else if ( wAddr < CHIP8_FONT_TOP ) {
    return RAM[ wAddr - 0x200 ];
  } else {
    return HEXFONT[ wAddr - CHIP8_FONT_TOP ];
  } 
}

WORD CPU_ReadW( WORD wAddr )
{
  return ( (WORD)CPU_Read( wAddr ) << 8 ) | CPU_Read( wAddr + 1 );
}

void CPU_Write( WORD wAddr, BYTE byData )
{
  //printf("Write:ADDR:%04x,DATA:%04x\n", wAddr, byData);
  if ( wAddr < 0x200 ) {
    return;
  } else if ( wAddr < CHIP8_FONT_TOP ) {
    RAM[ wAddr - 0x200 ] = byData;
    return; 
  }
}
