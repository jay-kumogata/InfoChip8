/*-------------------------------------------------------------------*/
/*  APU Resources                                                    */
/*-------------------------------------------------------------------*/

#define SAMPLE_RATE         44100
#define SAMPLES             ( SAMPLE_RATE / 60 )

extern BYTE Beep[ SAMPLES ];
extern BYTE None[ SAMPLES ];

/*-------------------------------------------------------------------*/
/*  APU Functions                                                    */
/*-------------------------------------------------------------------*/

/* APU Initialize */
extern int APU_Init( void );

/* APU Finalialize */
extern int APU_Fin( void );

/* Beep On */
#define APU_BeepOn() { \
  InfoChip8_SoundOutput( SAMPLES, Beep ); \
}

/* Beep Off */
#define APU_BeepOff() { \
  InfoChip8_SoundOutput( SAMPLES, None ); \
}
