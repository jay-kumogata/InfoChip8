#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

#include "../InfoChip8.h"
#include "../System.h"
#include "../CPU.h"
#include "../PPU.h"
#include "../IO.h"
#include "Sound_w32.h"

/*-------------------------------------------------------------------*/
/*  ROM image file information                                       */
/*-------------------------------------------------------------------*/
char szRomName[ 256 ];

/*-------------------------------------------------------------------*/
/*  Windows specific                                                 */
/*-------------------------------------------------------------------*/

#define APP_NAME           "InfoChip8 v0.1J"

#define CHIP8_MENU_WIDTH   8
#define CHIP8_MENU_HEIGHT  34

/* Variables for PPU Emulation */
HWND hWnd;
byte *pScreenMem;
HBITMAP hScreenBmp;
LOGPALETTE *plpal;
BITMAPINFO *bmi;

/* Variables for APU Emulation */
DIRSOUND* lpSndDevice = NULL;

/* Variables for Emulation Thread */
HANDLE m_hThread;
DWORD m_ThreadID = 0;

/* Function Prototype */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int CreateScreen( void );
void DestroyScreen( void );
int Init( void );
int Fin( void );

/*-------------------------------------------------------------------*/
/*  Main Routine                                                     */
/*-------------------------------------------------------------------*/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
		    LPSTR lpCmdLine, int nCmdShow )
{
  WNDCLASSEX wcex; 
 
  wcex.cbSize = sizeof(wcex);
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = "INFOCHIP8";
  wcex.hIconSm = NULL;
  
  if (!RegisterClassEx(&wcex))
    return FALSE; 

  hWnd = CreateWindow("INFOCHIP8", APP_NAME, WS_OVERLAPPEDWINDOW,
		      CW_USEDEFAULT, CW_USEDEFAULT,
		      CHIP8_WIDTH * 4 + CHIP8_MENU_WIDTH,
		      CHIP8_HEIGHT * 4 + CHIP8_MENU_HEIGHT,
		      NULL, NULL, hInstance, NULL);

  if (!hWnd)
    return FALSE;

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  /*-------------------------------------------------------------------*/
  /*  Init Resources                                                   */
  /*-------------------------------------------------------------------*/
  Init();

  /*-------------------------------------------------------------------*/
  /*  Main Loop                                                        */
  /*-------------------------------------------------------------------*/

  /* ROM Name */
  strncpy( szRomName, lpCmdLine, sizeof szRomName );

  /* Create Emulation Thread */
  m_hThread = CreateThread( (LPSECURITY_ATTRIBUTES)NULL, (DWORD)0,
			    (LPTHREAD_START_ROUTINE)InfoChip8_Main, 
			    (LPVOID)NULL, (DWORD)0, &m_ThreadID);

  /* Message Pump */
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return msg.wParam;
}

/*-------------------------------------------------------------------*/
/*  Callback Function                                                */
/*-------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  HDC hdc;
	
  switch (message) {
  case WM_DESTROY:
    Fin();
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

/*-------------------------------------------------------------------*/
/*  Initialize Resources                                             */
/*-------------------------------------------------------------------*/
int Init()
{
  /* Create Screen */
  CreateScreen();

  /* Random Seeds */
  srand( time( 0 ) ); 

  return 0;
}

/*-------------------------------------------------------------------*/
/*  Finalize Resources                                               */
/*-------------------------------------------------------------------*/
int Fin()
{
  /* Terminate Emulation Thread */
  Esc = 0x01; Sleep( 1000 );

  /* RAM */
  if ( RAM ) { free( RAM ); }

  /* Destroy Screen */
  DestroyScreen();

  return 0;
}

/*-------------------------------------------------------------------*/
/*  Create a InfoChip8 screen                                        */
/*-------------------------------------------------------------------*/
int CreateScreen( void )
{
  HDC hDC = GetDC( hWnd );
  
  BITMAPINFOHEADER bi;
  
  bi.biSize = sizeof( BITMAPINFOHEADER );
  bi.biWidth = CHIP8_WIDTH;
  bi.biHeight = CHIP8_HEIGHT * -1;
  bi.biPlanes = 1;
  bi.biBitCount = 16;
  bi.biCompression = BI_RGB;
  bi.biSizeImage = CHIP8_WIDTH * CHIP8_HEIGHT * 2;
  bi.biXPelsPerMeter = 0;
  bi.biYPelsPerMeter = 0;
  bi.biClrUsed = 0;
  bi.biClrImportant = 0;
  
  hScreenBmp = CreateDIBSection( hDC, 
                                 (BITMAPINFO *)&bi,
                                 DIB_RGB_COLORS, 
                                 (void **)&pScreenMem, 
                                 0,
                                 0 ); 
  ReleaseDC( hWnd, hDC );

  if ( !hScreenBmp ) { return -1; } 
  else {  return 0; }
}

/*-------------------------------------------------------------------*/
/*  Destroy InfoChip8 screen                                         */
/*-------------------------------------------------------------------*/
void DestroyScreen()
{
  if ( !hScreenBmp ) { DeleteObject( hScreenBmp ); }
}

/*-------------------------------------------------------------------*/
/*  Load ROM image                                                   */
/*-------------------------------------------------------------------*/
int InfoChip8_LoadRom( const char *pszFileName )
{
  int len;
  FILE *fp;

  /* Open ROM file */
  fp = fopen( pszFileName, "rb" );
  if ( fp == NULL )
    return -1;

  /* Get ROM length */
  fseek( fp, 0, SEEK_END );
  len = ftell(fp);
  fseek( fp, 0, SEEK_SET );

  /* Allocate Memory for ROM Image */
  RAM = (BYTE *)malloc( len );
  fread( RAM, 1, len, fp );

  /* File close */
  fclose( fp );

  /* Successful */
  return 0;
}

/*-------------------------------------------------------------------*/
/*  Load Frame                                                       */
/*-------------------------------------------------------------------*/
void InfoChip8_LoadFrame( void )
{
/*
 *  Transfer the contents of work frame on the screen
 */

  // Set screen data
  memcpy( pScreenMem, VRAM, CHIP8_WIDTH * CHIP8_HEIGHT * 2 );

  // Screen update
  HDC hDC = GetDC( hWnd );

  HDC hMemDC = CreateCompatibleDC( hDC );

  HBITMAP hOldBmp = (HBITMAP)SelectObject( hMemDC, hScreenBmp );

  StretchBlt( hDC, 0, 0, CHIP8_WIDTH * 4, CHIP8_HEIGHT * 4, hMemDC, 
              0, 0, CHIP8_WIDTH, CHIP8_HEIGHT, SRCCOPY );

  SelectObject( hMemDC, hOldBmp );

  DeleteDC( hMemDC );
  ReleaseDC( hWnd, hDC );
}

/*-------------------------------------------------------------------*/
/*  Sound Open                                                       */
/*-------------------------------------------------------------------*/
int InfoChip8_SoundOpen( int samples_per_sync, int sample_rate ) 
{
  lpSndDevice = new DIRSOUND( hWnd );

  if ( !lpSndDevice->SoundOpen( samples_per_sync, sample_rate ) )
  {
    InfoChip8_MessageBox( "SoundOpen() Failed." );
    exit(0);
  }

  return(TRUE);
}

/*-------------------------------------------------------------------*/
/*  Sound Close                                                      */
/*-------------------------------------------------------------------*/
void InfoChip8_SoundClose( void ) 
{
  lpSndDevice->SoundClose();
  delete lpSndDevice;
  lpSndDevice = NULL;
}

/*-------------------------------------------------------------------*/
/*  Sound Output Waves                                               */
/*-------------------------------------------------------------------*/
void InfoChip8_SoundOutput( int samples, BYTE *wave )
{
  if (!lpSndDevice->SoundOutput( samples, wave ) ) {
    InfoChip8_MessageBox( "SoundOutput() Failed." );
    exit(0);
  }
}

/*-------------------------------------------------------------------*/
/*  Get a Key State                                                  */
/*-------------------------------------------------------------------*/
void InfoChip8_KeyState( void )
{
  /* Key 1 */
#if 0
  Key =   ( GetAsyncKeyState( '0' ) < 0 )         | \
        ( ( GetAsyncKeyState( '1' ) < 0 ) <<  1 ) | \
        ( ( GetAsyncKeyState( '2' ) < 0 ) <<  2 ) | \
        ( ( GetAsyncKeyState( '3' ) < 0 ) <<  3 ) | \
        ( ( GetAsyncKeyState( '4' ) < 0 ) <<  4 ) | \
        ( ( GetAsyncKeyState( '5' ) < 0 ) <<  5 ) | \
        ( ( GetAsyncKeyState( '6' ) < 0 ) <<  6 ) | \
        ( ( GetAsyncKeyState( '7' ) < 0 ) <<  7 ) | \
        ( ( GetAsyncKeyState( '8' ) < 0 ) <<  8 ) | \
        ( ( GetAsyncKeyState( '9' ) < 0 ) <<  9 ) | \
        ( ( GetAsyncKeyState( 'A' ) < 0 ) << 10 ) | \
        ( ( GetAsyncKeyState( 'B' ) < 0 ) << 11 ) | \
        ( ( GetAsyncKeyState( 'C' ) < 0 ) << 12 ) | \
        ( ( GetAsyncKeyState( 'D' ) < 0 ) << 13 ) | \
        ( ( GetAsyncKeyState( 'E' ) < 0 ) << 14 ) | \
        ( ( GetAsyncKeyState( 'F' ) < 0 ) << 15 );
#else
  Key =   ( GetAsyncKeyState( 'B' ) < 0 )         | \
        ( ( GetAsyncKeyState( '4' ) < 0 ) <<  1 ) | \
        ( ( GetAsyncKeyState( '5' ) < 0 ) <<  2 ) | \
        ( ( GetAsyncKeyState( '6' ) < 0 ) <<  3 ) | \
        ( ( GetAsyncKeyState( 'R' ) < 0 ) <<  4 ) | \
        ( ( GetAsyncKeyState( 'T' ) < 0 ) <<  5 ) | \
        ( ( GetAsyncKeyState( 'Y' ) < 0 ) <<  6 ) | \
        ( ( GetAsyncKeyState( 'F' ) < 0 ) <<  7 ) | \
        ( ( GetAsyncKeyState( 'G' ) < 0 ) <<  8 ) | \
        ( ( GetAsyncKeyState( 'H' ) < 0 ) <<  9 ) | \
        ( ( GetAsyncKeyState( 'V' ) < 0 ) << 10 ) | \
        ( ( GetAsyncKeyState( 'N' ) < 0 ) << 11 ) | \
        ( ( GetAsyncKeyState( '7' ) < 0 ) << 12 ) | \
        ( ( GetAsyncKeyState( 'U' ) < 0 ) << 13 ) | \
        ( ( GetAsyncKeyState( 'J' ) < 0 ) << 14 ) | \
        ( ( GetAsyncKeyState( 'M' ) < 0 ) << 15 );
#endif

#if 0
  /* Esc */
  Esc = ( GetAsyncKeyState( VK_ESCAPE )  < 0 );
#endif

  Sleep( 0 );
}

/*-------------------------------------------------------------------*/
/*  Message Box                                                      */
/*-------------------------------------------------------------------*/
void InfoChip8_MessageBox( char *pszMsg, ... )
{
  char pszErr[ 1024 ];
  va_list args;

  /* Create the message body */
  va_start( args, pszMsg );
  vsprintf( pszErr, pszMsg, args );  pszErr[ 1023 ] = '\0';
  va_end( args );

  /* Only for debug */
  MessageBox( hWnd, pszErr, APP_NAME, MB_OK | MB_ICONSTOP );
}

/*-------------------------------------------------------------------*/
/*  Random Number                                                    */
/*-------------------------------------------------------------------*/
int InfoChip8_Rand( void )
{
  return rand();
}

/*-------------------------------------------------------------------*/
/*  Wait Function                                                    */
/*-------------------------------------------------------------------*/
void InfoChip8_Wait( int n )
{
  for ( int i = 0; i < n; i++ ) {
    Sleep( 0 );
  }
}
