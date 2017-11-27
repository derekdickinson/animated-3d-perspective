/* This file is linked into project.c and supplies some of the basic
functions.  Most of functions in this module are adapted from the
Turbo C example BGIDEMO.C. */
#include <bios.h>
#include <dos.h>
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <graphics.h>

#define TRUE	1			/* Define some handy constants	*/
#define FALSE	0			/* Define some handy constants	*/

int    GraphDriver;		/* The Graphics device driver		*/
int    GraphMode;		/* The Graphics mode value		*/
double AspectRatio;		/* Aspect ratio of a pixel on the screen*/
int    MaxX, MaxY;		/* The maximum resolution of the screen */
int    MaxColors;		/* The maximum # of colors available	*/
int    ErrorCode;		/* Reports any graphics errors		*/
struct palettetype palette;		/* Used to read palette info	*/
int    texheight;

int grtop,grbot,grleft,grright,txtop,txbot,txleft,txright;

extern int actpage;

/*									*/
/*	Function prototypes						*/
/*									*/

void Initialize(void);
int  gprintf(int *xloc, int *yloc, char *fmt, ... );
int  gprintf2(int *xloc, int *yloc, char *fmt, ... );
void cleartextport(void);

/*									*/
/*	INITIALIZE: Initializes the graphics system and reports		*/
/*	any errors which occured.					*/
/*									*/

void Initialize(void)
{
  int xasp, yasp;			/* Used to read the aspect ratio*/

  GraphDriver = DETECT;			/* Request auto-detection	*/
  initgraph( &GraphDriver, &GraphMode, "" );
  ErrorCode = graphresult();		/* Read result of initialization*/
  if( ErrorCode != grOk ){		/* Error occured during init	*/
    printf(" Graphics System Error: %s\n", grapherrormsg( ErrorCode ) );
    exit( 1 );
  }
  GraphMode=1;				/* Set to 640x350 with two pages */
  setgraphmode(GraphMode);
  ErrorCode = graphresult();		/* Read result of initialization*/
  if( ErrorCode != grOk ){		/* Error occured during init	*/
    printf(" Graphics System Error: %s\n", grapherrormsg( ErrorCode ) );
    exit( 1 );
  }

  getpalette( &palette );		/* Read the palette from board	*/
  MaxColors = getmaxcolor() + 1;	/* Read maximum number of colors*/

  MaxX = getmaxx();
  MaxY = getmaxy(); /* Read size of screen		*/

  getaspectratio( &xasp, &yasp );	/* read the hardware aspect	*/
  AspectRatio = (double)xasp / (double)yasp; /* Get correction factor	*/

  texheight=textheight("H");

  settextstyle( DEFAULT_FONT, HORIZ_DIR, 1 );
  settextjustify( LEFT_TEXT, TOP_TEXT );
  setlinestyle( SOLID_LINE, 0, NORM_WIDTH );
  setfillstyle( EMPTY_FILL, 0 );

  cleardevice();			/* Clear graphics screen	*/
  setcolor( 15 );

  /* Set up windows for text and graphics. */
  grtop=1;
  grbot=MaxY-texheight*3-3;
  grleft=1;
  grright=MaxX-1;

  txtop=MaxY-texheight*3-1;
  txbot=MaxY-1;
  txleft=1;
  txright=MaxX-1;

  /* Draw white borders on both windows. */
  setactivepage(1);
  setvisualpage(1);
  setviewport( 0, 0, MaxX, MaxY, 1 );	/* Open port to full screen	*/
  rectangle( 0, 0, MaxX, MaxY );

  line(0,grbot+1,MaxX,grbot+1 );
  setviewport( grleft, grtop, grright, grbot, 1 );

  setactivepage(0);
  setvisualpage(0);

  setviewport( 0, 0, MaxX, MaxY, 1 );	/* Open port to full screen	*/
  rectangle( 0, 0, MaxX, MaxY );

  line(0,grbot+1,MaxX,grbot+1 );
  setviewport( grleft, grtop, grright, grbot, 1 );
}

void cleartextport(void)
{
  setviewport( txleft, txtop, txright, txbot, 1 );

  clearviewport();
  setactivepage(!actpage);

  clearviewport();
  setactivepage(actpage);

  setviewport( grleft, grtop, grright, grbot, 1 );
}

/*									*/
/*	GPRINTF: Used like PRINTF except the output is sent to the	*/
/*	screen in graphics mode at the specified co-ordinate.		*/
/*									*/

int gprintf( int *xloc, int *yloc, char *fmt, ... )
{
  va_list  argptr;			/* Argument list pointer	*/
  char str[140];			/* Buffer to build sting into	*/
  int cnt;				/* Result of SPRINTF for return */

  setcolor( 15 );
  setviewport( txleft, txtop, txright, txbot, 1 );

  va_start( argptr, fmt );		/* Initialize va_ functions	*/

  cnt = vsprintf( str, fmt, argptr );	/* prints string to buffer	*/
  outtextxy( *xloc, *yloc, str );	/* Send string in graphics mode */
  *yloc += texheight + 2;	        /* Advance to next line		*/

  va_end( argptr );			/* Close va_ functions		*/

  setviewport( grleft, grtop, grright, grbot, 1 );

  return( cnt );			/* Return the conversion count	*/

}

/* Like gprintf except that the messages are written in both pages.  This
way the messages don't flash on and off. */
int gprintf2( int *xloc, int *yloc, char *fmt, ... )
{
  va_list  argptr;			/* Argument list pointer	*/
  char str[140];			/* Buffer to build sting into	*/
  int cnt;				/* Result of SPRINTF for return */

  va_start( argptr, fmt );		/* Initialize va_ functions	*/
  cnt = vsprintf( str, fmt, argptr );	/* prints string to buffer	*/

  setcolor( 15 );

  setviewport( txleft, txtop, txright, txbot, 1 );

  outtextxy( *xloc, *yloc, str );	/* Send string in graphics mode */

  setactivepage( !actpage );
  outtextxy( *xloc, *yloc, str );	/* Send string in graphics mode */

  setactivepage( actpage );

  setviewport( grleft, grtop, grright, grbot, 1 );

  *yloc += texheight + 2;	        /* Advance to next line		*/
  va_end( argptr );			/* Close va_ functions		*/

  return( cnt );			/* Return the conversion count	*/

}
