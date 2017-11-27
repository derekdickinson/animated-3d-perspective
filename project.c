/*
*/

/* Include Files */
#include <bios.h>
#include <dos.h>
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <graphics.h>

/* Constants */
#define TRUE  1
#define FALSE 0

#define MAXPOINTS 40
#define MAXLINES  70

/* These constants are passed to the rotateaxis function in order to specify
about which axis to spin. */
#define XAXIS  0
#define YAXIS  1
#define ZAXIS  2

/*
BLACK     BLUE          GREEN       CYAN
RED       MAGENTA       BROWN       LIGHTGRAY
DARKGRAY  LIGHTBLUE     LIGHTGREEN  LIGHTCYAN
LIGHTRED  LIGHTMAGENTA  YELLOW      WHITE

The above defines are made in graphics.h of Turbo C.  I use them below
to set different parts of the object to different colors.  The colors
can easily be changed by changing these defines.
*/
#define L1       LIGHTCYAN
#define L2       LIGHTGREEN
#define L3       LIGHTRED
#define S1       LIGHTGRAY
#define S2       LIGHTGRAY

#define TOPCOL   LIGHTCYAN
#define SIDCOL   LIGHTRED
#define BOTCOL   LIGHTGREEN

/* A little overkill on the significant digits here. */
#define PI     3.14159265358979323846

/* A convenient macro.  It complements boolean variables. */
#define NOT(x) (x)^=0x01

/* Type Definitions */
typedef unsigned char byte;
typedef unsigned char boolean;

typedef struct
{
  double xcord,ycord,zcord;
} POINT;

typedef struct
{
  int start,end,color; /* Each line has its own color. */
} LINE;

/* This structure is used to define the figure. */
typedef struct
{
  int numpoints, numlines;
  POINT point[MAXPOINTS];
  LINE line[MAXLINES];
} MYPOLY;

/* These are function prototypes which enable type checking. */
void Initialize(void);
int  gprintf2(int *xloc, int *yloc, char *fmt, ... );
void cleartextport(void);
void putpoly( MYPOLY *showit );

void dynademo3D(void);
void putnsay(MYPOLY *thepoly, char *message);

void scale(MYPOLY *thepoly, double xscale, double yscale, double zscale);
void trans(MYPOLY *thepoly, double x, double y, double z);
void rotateaxis(MYPOLY *thepoly, double theta,byte axis );
void rotateline(MYPOLY *thepoly, double theta, POINT one, POINT two);

void setparaview(double theta, double );
void parallel(MYPOLY *thepoly);
void perspect(MYPOLY *thepoly);

void setupvideo(void);
void putpoly(MYPOLY *showit);
void delpoly(MYPOLY *showit);
void redraw( MYPOLY *newpoly, MYPOLY *oldpoly);
void cleardraw( MYPOLY *newpoly);

/* Globals */
int    GraphDriver;		/* The Graphics device driver		*/
int    GraphMode;		/* The Graphics mode value		*/
double AspectRatio;		/* Aspect ratio of a pixel on the screen*/
int    MaxX, MaxY;		/* The maximum resolution of the screen */
int    MaxColors;		/* The maximum # of colors available	*/
int    ErrorCode;		/* Reports any graphics errors		*/
struct palettetype palette;		/* Used to read palette info	*/
int    texheight;

int grtop,grbot,grleft,grright,txtop,txbot,txleft,txright;

/* Pointer to the function which will perform the current viewing
transformation.  This value is reassigned to allow easy changing
from parallel to perspective transformation and back. */
void (near *viewtran)(MYPOLY *thepoly);

/* These values define the mapping from world coordinate to viewing
coordinates. */
int x_origin_map,y_origin_map;
double x_scale_map,y_scale_map;

/* These variables are used for the 3D to 2D viewing transformations.*/
double xadj,yadj,paral,paratheta,dist;
boolean persp=TRUE;

/* This variable defines the current active page. */
int actpage=0;

unsigned int millis=0;
unsigned int frate=0;

/* Currently active object and the maximum valid object. */
byte object=2;

/* The array of possible objects.  The data structure is defined in
"project.inc". */
MYPOLY thepolys[]=
{
  {    /* Three dimensional object from midterm. */
    36, 60,
    {
      {  0.0,   2.0,  0.0 },  { -1.0,   1.0,  0.0 },  { -2.5,   1.0,  0.0 },
      { -1.75,  0.0,  0.0 },  { -2.5,  -1.0,  0.0 },  { -1.0,  -1.0,  0.0 },
      {  0.0,  -2.0,  0.0 },  {  1.0,  -1.0,  0.0 },  {  2.5,  -1.0,  0.0 },
      {  1.75,  0.0,  0.0 },  {  2.5,   1.0,  0.0 },  {  1.0,   1.0,  0.0 },
      {  0.0,   1.0,  1.0 },  { -0.5,   0.5,  1.0 },  { -1.25,  0.5,  1.0 },
      { -0.875, 0.0,  1.0 },  { -1.25, -0.5,  1.0 },  { -0.5,  -0.5,  1.0 },
      {  0.0,  -1.0,  1.0 },  {  0.5,  -0.5,  1.0 },  {  1.25, -0.5,  1.0 },
      {  0.875, 0.0,  1.0 },  {  1.25,  0.5,  1.0 },  {  0.5,   0.5,  1.0 },
      {  0.0,   2.0,  2.0 },  { -1.0,   1.0,  2.0 },  { -2.5,   1.0,  2.0 },
      { -1.75,  0.0,  2.0 },  { -2.5,  -1.0,  2.0 },  { -1.0,  -1.0,  2.0 },
      {  0.0,  -2.0,  2.0 },  {  1.0,  -1.0,  2.0 },  {  2.5,  -1.0,  2.0 },
      {  1.75,  0.0,  2.0 },  {  2.5,   1.0,  2.0 },  {  1.0,   1.0,  2.0 }
    },
    {
      {  0,  1, L1 }, {  1,  2, L1 }, {  2,  3, L1 }, {  3,  4, L1 },
      {  4,  5, L1 }, {  5,  6, L1 }, {  6,  7, L1 }, {  7,  8, L1 },
      {  8,  9, L1 }, {  9, 10, L1 }, { 10, 11, L1 }, { 11,  0, L1 },

      { 12, 13, L2 }, { 13, 14, L2 }, { 14, 15, L2 }, { 15, 16, L2 },
      { 16, 17, L2 }, { 17, 18, L2 }, { 18, 19, L2 }, { 19, 20, L2 },
      { 20, 21, L2 }, { 21, 22, L2 }, { 22, 23, L2 }, { 23, 12, L2 },

      { 24, 25, L3 }, { 25, 26, L3 }, { 26, 27, L3 }, { 27, 28, L3 },
      { 28, 29, L3 }, { 29, 30, L3 }, { 30, 31, L3 }, { 31, 32, L3 },
      { 32, 33, L3 }, { 33, 34, L3 }, { 34, 35, L3 }, { 35, 24, L3 },

      {  0, 12, S1 }, {  1, 13, S1 }, {  2, 14, S1 }, {  3, 15, S1 },
      {  4, 16, S1 }, {  5, 17, S1 }, {  6, 18, S1 }, {  7, 19, S1 },
      {  8, 20, S1 }, {  9, 21, S1 }, { 10, 22, S1 }, { 11, 23, S1 },

      { 12, 24, S2 }, { 13, 25, S2 }, { 14, 26, S2 }, { 15, 27, S2 },
      { 16, 28, S2 }, { 17, 29, S2 }, { 18, 30, S2 }, { 19, 31, S2 },
      { 20, 32, S2 }, { 21, 33, S2 }, { 22, 34, S2 }, { 23, 35, S2 }
    }
  },
  {         /* A single star from the object above. */
    12, 12,
    {
      {  0.0,   2.0,  0.0 },  { -1.0,   1.0,  0.0 },  { -2.5,   1.0,  0.0 },
      { -1.75,  0.0,  0.0 },  { -2.5,  -1.0,  0.0 },  { -1.0,  -1.0,  0.0 },
      {  0.0,  -2.0,  0.0 },  {  1.0,  -1.0,  0.0 },  {  2.5,  -1.0,  0.0 },
      {  1.75,  0.0,  0.0 },  {  2.5,   1.0,  0.0 },  {  1.0,   1.0,  0.0 }
    },
    {
      {  0,  1, L1 }, {  1,  2, L1 }, {  2,  3, L1 }, {  3,  4, L1 },
      {  4,  5, L1 }, {  5,  6, L1 }, {  6,  7, L1 }, {  7,  8, L1 },
      {  8,  9, L1 }, {  9, 10, L1 }, { 10, 11, L1 }, { 11,  0, L1 }
    }
  },
  {         /* Cube */
    8, 12,
    {
      {  1.0,   1.0,  1.0 },
      {  1.0,  -1.0,  1.0 },
      { -1.0,  -1.0,  1.0 },
      { -1.0,   1.0,  1.0 },
      {  1.0,   1.0, -1.0 },
      {  1.0,  -1.0, -1.0 },
      { -1.0,  -1.0, -1.0 },
      { -1.0,   1.0, -1.0 }
    },
    {
      { 0, 1, TOPCOL }, { 1, 2, TOPCOL }, { 2, 3, TOPCOL }, { 3, 0, TOPCOL },
      { 4, 5, BOTCOL }, { 5, 6, BOTCOL }, { 6, 7, BOTCOL }, { 7, 4, BOTCOL },
      { 0, 4, SIDCOL }, { 1, 5, SIDCOL }, { 2, 6, SIDCOL }, { 3, 7, SIDCOL }
    }
  },
  {         /* Tetrahedron */
    5, 8,
    {
      {  1.0,   0.0,  1.0 },
      {  1.0,   0.0, -1.0 },
      { -1.0,   0.0, -1.0 },
      { -1.0,   0.0,  1.0 },
      {  0.0,   2.0,  0.0 }
    },
    {
      { 0, 1, TOPCOL }, { 1, 2, TOPCOL }, { 2, 3, TOPCOL }, { 3, 0, TOPCOL },
      { 0, 4, SIDCOL }, { 1, 4, BOTCOL }, { 2, 4, SIDCOL }, { 3, 4, BOTCOL }
    }
  },
  {         /* A can't describe this one. */
    8, 12,
    {
      {  1.0,   1.0,  1.0 },
      {  1.0,  -1.0,  1.0 },
      { -1.0,  -1.0,  1.0 },
      { -1.0,   1.0,  1.0 },
      {  1.0,   1.0, -1.0 },
      {  1.0,  -1.0, -1.0 },
      { -1.0,  -1.0, -1.0 },
      { -1.0,   1.0, -1.0 }
    },
    {
      { 0, 1, TOPCOL }, { 1, 2, TOPCOL }, { 2, 3, TOPCOL }, { 3, 0, TOPCOL },
      { 0, 5, SIDCOL }, { 1, 6, SIDCOL }, { 2, 7, SIDCOL }, { 3, 4, SIDCOL },
      { 0, 4, BOTCOL }, { 1, 5, BOTCOL }, { 2, 6, BOTCOL }, { 3, 7, BOTCOL }
    }
  },
  {         /* Cube with crossed ends. */
    8, 16,
    {
      {  1.0,   1.0,  1.0 },
      {  1.0,  -1.0,  1.0 },
      { -1.0,  -1.0,  1.0 },
      { -1.0,   1.0,  1.0 },
      {  1.0,   1.0, -1.0 },
      {  1.0,  -1.0, -1.0 },
      { -1.0,  -1.0, -1.0 },
      { -1.0,   1.0, -1.0 }
    },
    {
      { 0, 1, TOPCOL }, { 1, 2, TOPCOL }, { 2, 3, TOPCOL }, { 3, 0, TOPCOL },
      { 4, 5, BOTCOL }, { 5, 6, BOTCOL }, { 6, 7, BOTCOL }, { 7, 4, BOTCOL },
      { 0, 4, SIDCOL }, { 1, 5, SIDCOL }, { 2, 6, SIDCOL }, { 3, 7, SIDCOL },

      { 0, 2, TOPCOL }, { 1, 3, TOPCOL }, { 4, 6, BOTCOL }, { 5, 7, BOTCOL }
    }
  },
  {         /*  Similar to above except the sides are removed. */
    10, 17,
    {
      {  1.0,   1.0,  1.0 },
      {  1.0,  -1.0,  1.0 },
      { -1.0,  -1.0,  1.0 },
      { -1.0,   1.0,  1.0 },
      {  1.0,   1.0, -1.0 },
      {  1.0,  -1.0, -1.0 },
      { -1.0,  -1.0, -1.0 },
      { -1.0,   1.0, -1.0 },
      {  0.0,   1.0,  0.0 },
      {  0.0,  -1.0,  0.0 }
    },
    {
      { 0, 1, TOPCOL }, { 1, 2, TOPCOL }, { 2, 3, TOPCOL }, { 3, 0, TOPCOL },
      { 4, 5, BOTCOL }, { 5, 6, BOTCOL }, { 6, 7, BOTCOL }, { 7, 4, BOTCOL },
      { 0, 2, TOPCOL }, { 1, 3, TOPCOL }, { 4, 6, BOTCOL }, { 5, 7, BOTCOL },
      { 8, 9, SIDCOL }
    }
  },
  {         /* Square and a line through it.  Can be drawn very quickly. */
    6, 5,
    {
      {  1.0,  0.0,  1.0 },
      {  1.0,  0.0, -1.0 },
      { -1.0,  0.0, -1.0 },
      { -1.0,  0.0,  1.0 },

      {  0.0,   1.0,  0.0 },
      {  0.0,  -1.0,  0.0 },
    },
    {
      { 0, 1, TOPCOL }, { 1, 2, TOPCOL }, { 2, 3, TOPCOL }, { 3, 0, TOPCOL },
      { 4, 5, BOTCOL }
    }
  },
  {         /* Cube with a little stick man in it. */
    14, 16,
    {
      {  1.0,   1.0,  1.0 },
      {  1.0,  -1.0,  1.0 },
      { -1.0,  -1.0,  1.0 },
      { -1.0,   1.0,  1.0 },
      {  1.0,   1.0, -1.0 },
      {  1.0,  -1.0, -1.0 },
      { -1.0,  -1.0, -1.0 },
      { -1.0,   1.0, -1.0 },
      { -1.0,  -1.0, -0.2 },
      { -1.0,  -1.0,  0.2 },
      { -0.5,  -0.5,  0.0 },
      {  0.25,  0.25, 0.0 },
      { -0.125, -0.125, -0.25},
      { -0.125, -0.125,  0.25}
    },
    {
      { 0, 1, TOPCOL }, { 1, 2, TOPCOL }, { 2, 3, TOPCOL }, { 3, 0, TOPCOL },
      { 4, 5, BOTCOL }, { 5, 6, BOTCOL }, { 6, 7, BOTCOL }, { 7, 4, BOTCOL },
      { 0, 4, SIDCOL }, { 1, 5, SIDCOL }, { 2, 6, SIDCOL }, { 3, 7, SIDCOL },
      { 8,10, WHITE  }, { 9,10, WHITE  }, { 10,11, WHITE  }, { 12, 13, WHITE }
    }
  },
  {         /* Cube with a different stick man in it. */
    14, 16,
    {
      {  1.0,   1.0,  1.0 },
      {  1.0,  -1.0,  1.0 },
      { -1.0,  -1.0,  1.0 },
      { -1.0,   1.0,  1.0 },
      {  1.0,   1.0, -1.0 },
      {  1.0,  -1.0, -1.0 },
      { -1.0,  -1.0, -1.0 },
      { -1.0,   1.0, -1.0 },
      {  0.25,  0.25,-0.3 },
      {  0.25,  0.25, 0.3 },
      { -1.0,  -1.0, -0.15 },
      { -1.0,  -1.0,  0.15 },
      { -0.125, -0.125, -0.4 },
      { -0.125, -0.125,  0.4 }
    },
    {
      { 0, 1, TOPCOL }, { 1, 2, TOPCOL }, { 2, 3, TOPCOL }, { 3, 0, TOPCOL },
      { 4, 5, BOTCOL }, { 5, 6, BOTCOL }, { 6, 7, BOTCOL }, { 7, 4, BOTCOL },
      { 0, 4, SIDCOL }, { 1, 5, SIDCOL }, { 2, 6, SIDCOL }, { 3, 7, SIDCOL },
      { 8, 9, WHITE  }, { 9,10, WHITE  }, { 8,11, WHITE  }, { 12, 13, WHITE }
    }
  },
  {         /* Solo stick man. */
    6, 4,
    {
      {  0.25,  0.25,-0.3 },
      {  0.25,  0.25, 0.3 },
      { -1.0,  -1.0, -0.15 },
      { -1.0,  -1.0,  0.15 },
      { -0.125, -0.125, -0.4 },
      { -0.125, -0.125,  0.4 }
    },
    {
      { 0, 1, WHITE  }, { 1,2, WHITE  }, { 0,3, WHITE  }, { 4, 5, WHITE }
    }
  }
};

/* A count of the number of objects. */
byte numobj=sizeof(thepolys)/sizeof(MYPOLY);

/* Pointer to currently active object. */
MYPOLY *currentpoly=&thepolys[2];

void WinMain()
{
  Initialize();	/* Set system into Graphics mode     */

  setupvideo(); /* Set up scale factors and projection values. */

  dynademo3D(); /* The dynamic routine. */

  closegraph();	/* Return the system to text mode	*/
}

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

/*****************************************************************************
This function is the main control function for the program.

There are several major sections to this function:

  1. Displaying of current status (the text window at the bottom).
  2. The loop which modifies the data structure and redisplays it.
  3. The keyboard input section that gets keys and changes values.

All three of these are in a loop which runs until the user types Esc or 'Q'.

*****************************************************************************/

/* The line about which the object is rotated.  This line is displayed if
rotation about an arbritrary axis is enabled. */
MYPOLY theline=
{
  2,1,
  {
    { 0.0, 5.0, 5.0}, { -5.0, -5.0, 0.0 }
  },
  {
    { 0,1,15 }
  }
};

/* The ruling box for translations */
MYPOLY thebox=
{
  8, 12,
  {
    {  5.0,   4.0,  6.0 },
    {  5.0,  -4.0,  6.0 },
    { -5.0,  -4.0,  6.0 },
    { -5.0,   4.0,  6.0 },
    {  5.0,   4.0, -1.5 },
    {  5.0,  -4.0, -1.5 },
    { -5.0,  -4.0, -1.5 },
    { -5.0,   4.0, -1.5 }
  },
  {
    { 0, 1, 15 }, { 1, 2, 15 }, { 2, 3, 15 }, { 3, 0, 15 },
    { 4, 5, 15 }, { 5, 6, 15 }, { 6, 7, 15 }, { 7, 4, 15 },
    { 0, 4, 15 }, { 1, 5, 15 }, { 2, 6, 15 }, { 3, 7, 15 }
  }
};

/* These arrays of pointers point to the values which can be changed in the
key input section to modify the transformations. */
double *changxvar[9];
double *changyvar[9];
double *changzvar[9];
double changval[9]={ 0.5, 0.25, 0.25, 0.1, 0.25, 0.25, 0.05, 0.2, 0.2 };

/*-------------- Locally used macros. -----------*/

/* These macros are used to update the text window. */

#define STAT0() gprintf2( &x, &y,"Spins [[ %4.2f, %4.2f, %4.2f ]] ",\
				Xrodel, Yrodel, Zrodel)

#define STAT1() gprintf2( &x, &y,"Rotate [[ %4.2f, %4.2f, %4.2f ]] - ( %4.2f, %4.2f, %4.2f ), %4.2f",\
     one.xcord, one.ycord, one.zcord, two.xcord, two.ycord, two.zcord, rodel)

#define STAT2() gprintf2( &x, &y,"Rotate ( %4.2f, %4.2f, %4.2f ) - [[ %4.2f, %4.2f, %4.2f ]], %4.2f",\
	  one.xcord, one.ycord, one.zcord, two.xcord, two.ycord, two.zcord, rodel)

#define STAT3() gprintf2( &x, &y,"Translate [[ %4.2f, %4.2f, %4.2f ]] ( %4.2f, %4.2f, %4.2f ) ( %4.2f, %4.2f, %4.2f ) ",\
	  txdel, tydel, tzdel, btxmin, btymin, btzmin, btxmax, btymax, btzmax)

#define STAT4() gprintf2( &x, &y,"Translate ( %4.2f, %4.2f, %4.2f ) [[ %4.2f, %4.2f, %4.2f ]] ( %4.2f, %4.2f, %4.2f ) ",\
	  txdel, tydel, tzdel, btxmin, btymin, btzmin, btxmax, btymax, btzmax)

#define STAT5() gprintf2( &x, &y,"Translate ( %4.2f, %4.2f, %4.2f ) ( %4.2f, %4.2f, %4.2f ) [[ %4.2f, %4.2f, %4.2f ]] ",\
	  txdel, tydel, tzdel, btxmin, btymin, btzmin, btxmax, btymax, btzmax)

#define STAT6() gprintf2( &x, &y,"Scaling [[ %4.2f, %4.2f, %4.2f ]] ( %4.2f, %4.2f, %4.2f ) ( %4.2f, %4.2f, %4.2f ) ",\
	  sxdel, sydel, szdel, sxmin, symin, szmin, sxmax, symax, szmax)

#define STAT7() gprintf2( &x, &y,"Scaling ( %4.2f, %4.2f, %4.2f ) [[ %4.2f, %4.2f, %4.2f ]] ( %4.2f, %4.2f, %4.2f ) ",\
	  sxdel, sydel, szdel, sxmin, symin, szmin, sxmax, symax, szmax)

#define STAT8() gprintf2( &x, &y,"Scaling ( %4.2f, %4.2f, %4.2f ) ( %4.2f, %4.2f, %4.2f ) [[ %4.2f, %4.2f, %4.2f ]] ",\
	  sxdel, sydel, szdel, sxmin, symin, szmin, sxmax, symax, szmax)

#define STATSET( ind, bool, ch )  status[ind]= (bool) ? ch : ' '

/* Macros for the rotation function. */
#define POINTx(i,poly) poly->point[i].xcord
#define POINTy(i,poly) poly->point[i].ycord
#define POINTz(i,poly) poly->point[i].zcord

/*-------------- End of locally used macros. -----------*/

boolean
	  /* These booleans indicate if the particular transformation is
	  enabled. */
	  rotate=FALSE,translat=TRUE,scal=FALSE,
	  rotateX=FALSE,rotateY=FALSE,rotateZ=FALSE,
	  autorate=TRUE;

double
	 /* The change between frames of the angle values. */
	 rodel=5.0,Xrodel=0.0,Yrodel=0.0,Zrodel=0.0,

	 /* The change between frames of the translation and scaling values. */
	 txdel=0, tydel=0, tzdel=.1, sxdel=0, sydel=0, szdel=0,

	 /* The bounds of scaling and translations. */
	 txmax=5.25, tymax=4.0, tzmax=6.0, sxmax=1, symax=1, szmax=1,
	 txmin=-5.25, tymin=-4.0, tzmin=-1.5, sxmin=1, symin=1, szmin=1,

	 /* The bounds of scaling and translations. */
	 btxmax=6.25, btymax=4.0, btzmax=14.5,
	 btxmin=-6.25, btymin=-4.0, btzmin=-0.25;

POINT one, two; /* Endpoints of line for rotation about arbritrary axis. */

char changtype=3;

void showstat(void)
{
  int x, y;
  char status[20];

  x=10; y=4; cleartextport();

  /* Display top line. */
  switch (changtype)
  {
    case 0: STAT0(); break;  case 1: STAT1(); break;  case 2: STAT2(); break;
    case 3: STAT3(); break;  case 4: STAT4(); break;  case 5: STAT5(); break;
    case 6: STAT6(); break;  case 7: STAT7(); break;  case 8: STAT8(); break;
  }

  /* Setup status line containing active transformations. */
  memset( status,0x20,sizeof(status));
  STATSET( 0, scal,    'S'); STATSET( 2, rotateX, 'X');
  STATSET( 4, rotateY, 'Y'); STATSET( 6, rotateZ, 'Z');
  STATSET( 8, rotate,  'R'); STATSET(10, translat,'T'); status[11]=0;

  /* Display second line. */
  if (persp)
    gprintf2(&x,&y,"status =%s | Perspective, distance=%4.2f | Millis=%d | Rate=%d ",
		    status,                      dist,         millis,  frate);
  else
    gprintf2(&x,&y,"status =%s | Parallel, length = %4.2f, angle = %4.2f | Millis=%d | Rate=%d ",
		    status,                   paral,       paratheta,     millis,  frate);

}

/* Now the function itself. */
void dynademo3D(void)
{

  /*-------------- Local data declarations. -----------*/

  /* Variables for text window display. */
  boolean
	  done,           /* False until the user wants to exit. */
	  firstime=TRUE;  /* True if the user pressed a key since last
			     graphic write. */

  double
	 /* The current angle values for the rotations. */
	 theta=0.0,Xtheta=0.0,Ytheta=0.0,Ztheta=0.0,

	 /* Current values for translation and scaling. */
	 tx=-5.25,    ty=-3.0,    tz=4.0,    sx=1,    sy=1,    sz=1;

  /* For ef  */
  MYPOLY buf0, buf1, buf2, *newpoly, *dispoly, *backpoly, *temp;


 /*-------------- End of local data declarations. -----------*/

  /* Initialize arrays to determine which values are modified with X,Y and Z
  keys. */
  changxvar[0]=&Xrodel;  changxvar[1]=&one.xcord;  changxvar[2]=&two.xcord;
  changxvar[3]=&txdel;   changxvar[4]=&btxmin;     changxvar[5]=&btxmax;
  changxvar[6]=&sxdel;   changxvar[7]=&sxmin;      changxvar[8]=&sxmax;

  changyvar[0]=&Yrodel;  changyvar[1]=&one.ycord;  changyvar[2]=&two.ycord;
  changyvar[3]=&tydel;   changyvar[4]=&btymin;     changyvar[5]=&btymax;
  changyvar[6]=&sydel;   changyvar[7]=&symin;      changyvar[8]=&symax;

  changzvar[0]=&Zrodel;  changzvar[1]=&one.zcord;  changzvar[2]=&two.zcord;
  changzvar[3]=&tzdel;   changzvar[4]=&btzmin;     changzvar[5]=&btzmax;
  changzvar[6]=&szdel;   changzvar[7]=&szmin;      changzvar[8]=&szmax;

  /* Set up both text and view windows. */
  cleartextport();
  clearviewport();

  one=theline.point[0];
  two=theline.point[1];

  memmove(&buf0,currentpoly,sizeof(MYPOLY));
  memmove(&buf1,currentpoly,sizeof(MYPOLY));
  memmove(&buf2,currentpoly,sizeof(MYPOLY));

  newpoly=&buf0;
  dispoly=&buf1;
  backpoly=&buf2;

  for ( done=FALSE; !done;)
  {

    /* Draw rotation line if necessary */
    if (rotate)
    {
      theline.point[0]=one;
      theline.point[1]=two;
      viewtran(&theline);
      cleardraw(&theline);
    }
    else
    {
      delpoly( &theline );
      setactivepage(!actpage);
      delpoly( &theline );
      setactivepage(actpage);
    }

    /* Draw translation box if necessary */
    if (translat)
    {
      int i;
      double maxx=-500,maxy=-500,maxz=-500,minx=500,miny=500,minz=500;

      for (i=0; i<currentpoly->numpoints ; i++)
      {
	 maxx = (POINTx(i,currentpoly)>maxx) ? POINTx(i,currentpoly) : maxx ;
	 maxy = (POINTy(i,currentpoly)>maxy) ? POINTy(i,currentpoly) : maxy ;
	 maxz = (POINTz(i,currentpoly)>maxz) ? POINTz(i,currentpoly) : maxz ;
	 minx = (POINTx(i,currentpoly)<minx) ? POINTx(i,currentpoly) : minx ;
	 miny = (POINTy(i,currentpoly)<miny) ? POINTy(i,currentpoly) : miny ;
	 minz = (POINTz(i,currentpoly)<minz) ? POINTz(i,currentpoly) : minz ;
      }

      thebox.point[0].xcord=thebox.point[1].xcord=
	thebox.point[4].xcord=thebox.point[5].xcord=btxmax;

      thebox.point[2].xcord=thebox.point[3].xcord=
	thebox.point[6].xcord=thebox.point[7].xcord=btxmin;

      thebox.point[0].ycord=thebox.point[3].ycord=
	thebox.point[4].ycord=thebox.point[7].ycord=btymax;

      thebox.point[1].ycord=thebox.point[2].ycord=
	thebox.point[5].ycord=thebox.point[6].ycord=btymin;

      thebox.point[0].zcord=thebox.point[1].zcord=
	thebox.point[2].zcord=thebox.point[3].zcord=btzmax;

      thebox.point[4].zcord=thebox.point[5].zcord=
	thebox.point[6].zcord=thebox.point[7].zcord=btzmin;

      txmax=btxmax-maxx;
      tymax=btymax-maxy;
      tzmax=btzmax-maxz;
      txmin=btxmin-minx;
      tymin=btymin-miny;
      tzmin=btzmin-minz;

      if ( ( tx > txmax ) || ( tx < txmin ) ) tx=(txmax+txmin)/2;
      if ( ( ty > tymax ) || ( ty < tymin ) ) ty=(tymax+tymin)/2;
      if ( ( tz > tzmax ) || ( tz < tzmin ) ) tz=(tzmax+tzmin)/2;

      viewtran(&thebox);
      cleardraw(&thebox);
    }
    else
    {
      delpoly( &thebox );
      setactivepage(!actpage);
      delpoly( &thebox );
      setactivepage(actpage);
    }

    /*-------------- Status (text) display section. -----------*/

    showstat();

    /*-------------- End of status (text) display section. -----------*/

    /* This is the primary loop. */

    while ( !bioskey(1) )  /* Wait for a key to be pressed. */
    {
      memmove(newpoly,currentpoly,sizeof(MYPOLY));

      if (scal)
      {
	scale( newpoly, sx, sy, sz );
	sx+=sxdel; sy+=sydel; sz+=szdel;
	if ( ( sx > sxmax ) || ( sx < sxmin ) ) sxdel*=-1;
	if ( ( sy > symax ) || ( sy < symin ) ) sydel*=-1;
	if ( ( sz > szmax ) || ( sz < szmin ) ) szdel*=-1;
      }

      if (rotateX)
      {
	rotateaxis( newpoly, Xtheta, XAXIS );
	Xtheta += Xrodel;
	if (Xtheta>360.0) Xtheta-=360.0;
      }

      if (rotateY)
      {
	rotateaxis( newpoly, Ytheta, YAXIS );
	Ytheta += Yrodel;
	if (Ytheta>360.0) Ytheta-=360.0;
      }

      if (rotateZ)
      {
	rotateaxis( newpoly, Ztheta, ZAXIS );
	Ztheta += Zrodel;
	if (Ztheta>360.0) Ztheta-=360.0;
      }

      if (rotate)
      {
	rotateline( newpoly, theta, one, two );
	theta += rodel;
	if (theta>360.0) theta-=360.0;
	putpoly(&theline);
      }

      if (translat)
      {
	trans( newpoly, tx, ty, tz );
	tx+=txdel; ty+=tydel; tz+=tzdel;
	if ( ( tx > txmax ) || ( tx < txmin ) ) txdel*=-1;
	if ( ( ty > tymax ) || ( ty < tymin ) ) tydel*=-1;
	if ( ( tz > tzmax ) || ( tz < tzmin ) ) tzdel*=-1;
	putpoly(&thebox);
      }

      viewtran(newpoly);

      if (firstime)
      {
	cleardraw(newpoly);
	firstime=FALSE;
      }
      else redraw( newpoly, backpoly);

      temp=backpoly;

      backpoly=dispoly;

      dispoly=newpoly;

      newpoly=temp;

    }

    /*-------------- This section interprets keyboard input. -----------*/

    firstime=TRUE;

    /* Set delta values to positive to make sure change is in desired direction. */
    txdel=fabs(txdel); tydel=fabs(tydel); tzdel=fabs(tzdel);
    sxdel=fabs(sxdel); sydel=fabs(sydel); szdel=fabs(szdel);

    /* Make sure scale and translate values don't go 2 over max value. */
    if (tx>txmax) tx=(txmax-txdel); if (ty>tymax) ty=(tymax-tydel);
    if (tz>tzmax) tz=(tzmax-tzdel); if (sx>sxmax) sx=(sxmax-sxdel);
    if (sy>symax) sy=(symax-sydel); if (sz>szmax) sz=(szmax-szdel);

    /* Note: The bioskey function checks the value in the keyboard buffer
	     but does not take it out. */
    switch ( bioskey(1) & 0xff)
    {
      case 'Q': case 'q': case 0x1b: done=TRUE; break;

      /* These toggle the transformations on and off.*/
      case 13: NOT(autorate); break; /* Ctrl-M */
      case 18: NOT(rotate);   break; /* Ctrl-R */
      case 'S': case 's':
      case 19: NOT(scal);     break; /* Ctrl-S */
      case 'T': case 't':
      case 20: NOT(translat); break; /* Ctrl-T */
      case 24: NOT(rotateX);  break; /* Ctrl-X */
      case 25: NOT(rotateY);  break; /* Ctrl-Y */
      case 26: NOT(rotateZ);  break; /* Ctrl-Z */

      /* These adjust the change per frame (delta) or the maximum and minimum
      bounds of a transformation depending on changtype. */
      case 'R': rodel+=0.5;   break;
      case 'r': rodel-=0.5;   break;
      case 'X': *changxvar[changtype]+=changval[changtype]; break;
      case 'x': *changxvar[changtype]-=changval[changtype]; break;
      case 'Y': *changyvar[changtype]+=changval[changtype]; break;
      case 'y': *changyvar[changtype]-=changval[changtype]; break;
      case 'Z': *changzvar[changtype]+=changval[changtype]; break;
      case 'z': *changzvar[changtype]-=changval[changtype]; break;

      /* These commands adjust the viewing transformations. */
      case 'V': case 'v':
	viewtran= (persp) ? parallel : perspect;
	NOT(persp); break;
      case 'D': dist+=1.0; break;
      case 'd': dist-=1.0; break;
      case 'L': paral+=0.05;    setparaview( paral, paratheta ); break;
      case 'l': paral-=0.05;    setparaview( paral, paratheta ); break;

      case 'M': millis++; break;
      case 'm': millis--; break;

      case 'K': paratheta+=1.0; setparaview( paral, paratheta ); break;
      case 'k': paratheta-=1.0; setparaview( paral, paratheta ); break;

      case 'P':
	object=(++object % numobj);
	currentpoly=&thepolys[object]; break;
      case 'p':
	object=(object==0) ? numobj-1 : object-1;
	currentpoly=&thepolys[object]; break;

      default: /* Case 0 */
	switch ( bioskey(1) )
	{
	  case 0x4d00: case 0x4800:  /* Right or Up arrow. */
	    if (++changtype > 8) changtype=0; break;
	  case 0x4b00: case 0x5000:  /* Left or Down arrow. */
	    if (--changtype < 0) changtype=8; break;
	  case 0x1300: theta=0;     break; /* Alt-R */
	  case 0x1F00: sx=sy=sz=1;  break; /* Alt-S */
	  case 0x1400: tx=ty=tz=0;  break; /* Alt-T */
	  case 0x2D00: Xtheta=0;    break; /* Alt-X */
	  case 0x1500: Ytheta=0;    break; /* Alt-Y */
	  case 0x2C00: Ztheta=0;    break; /* Alt-Z */
	}
    }

    bioskey(0); /* Take the key out of the buffer. */

    /*-------------- End of keyboard input. -----------*/

  }
}

/*****************************************************************************
scaling, translations and rotations
*****************************************************************************/

void scale(MYPOLY *thepoly,double xscale,double yscale, double zscale)
{
  byte i;

  for(i=0; i < thepoly->numpoints; i++)
  {
    thepoly->point[i].xcord*=xscale;
    thepoly->point[i].ycord*=yscale;
    thepoly->point[i].zcord*=zscale;
  }
}

void trans(MYPOLY *thepoly, double x, double y, double z)
{
  int i;

  for(i=0; i < thepoly->numpoints ; i++)
  {
    thepoly->point[i].xcord+=x;
    thepoly->point[i].ycord+=y;
    thepoly->point[i].zcord+=z;
  }
}

/*
                          -- Note --
A commonly used trick in the program is to set a pointer to the proper
coordinate (x, y or z) in an array of points and then increment it by 3
locations to reference the like coordinate of the nest point.  This is
faster than indexing since the address calculation is simpler.  You will
see this used in several of the remaining transformations.
*/

/* Rotations around one of the primary axises. */
void rotateaxis(MYPOLY *thepoly, double theta,byte axis )
{
  byte i;
  double *dim0ptr,*dim1ptr;
  double dim0,sintheta, costheta;

  theta=theta*PI/180.0;

  /* Set pointers to start on the proper coordinate. */
  switch (axis)
  {
    case XAXIS:
      dim0ptr=&(thepoly->point[0].ycord);
      dim1ptr=&(thepoly->point[0].zcord);
      break;
    case YAXIS:
      dim0ptr=&(thepoly->point[0].xcord);
      dim1ptr=&(thepoly->point[0].zcord);
      theta*=-1;  /* Angle is in opposite direction for Y rotation. */
      break;
    case ZAXIS:
      dim0ptr=&(thepoly->point[0].xcord);
      dim1ptr=&(thepoly->point[0].ycord);
      break;
  }

  /* Calculate sines and cosines only once. */
  sintheta=sin(theta);
  costheta=cos(theta);

  /* Modify all the points.  Note: The pointers will stay on the coordinate
  dimension that they began on since they are incremented by three each time.
  This is a little harder to look at but is more flexible and is faster. */
  for( i=0; i<thepoly->numpoints; i++)
  {
    dim0=*dim0ptr;
    *dim0ptr=dim0 * costheta - *dim1ptr * sintheta;
    *dim1ptr=dim0 * sintheta + *dim1ptr * costheta;
    dim0ptr+=3;
    dim1ptr+=3;
  }

}

void rotateline(MYPOLY *thepoly, double theta,
		POINT one, POINT two)  /* End points of line. */
/* This logic is fundamentally out of the handout.  The implementation
is designed to be as efficient as possible. */
{
  byte i;
  double xdif,ydif,zdif,M;
  static double rxx,rxy,rxz,ryx,ryy,ryz,rzx,rzy,rzz,sintheta,costheta,x,y,z;
  static double *pointx,*pointy,*pointz;

  theta=theta*PI/180.0;

  xdif=(two.xcord-one.xcord);
  ydif=(two.ycord-one.ycord);
  zdif=(two.zcord-one.zcord);
  M=sqrt( xdif*xdif + ydif*ydif + zdif*zdif );

  costheta=cos(theta);
  sintheta=sin(theta);

  xdif/=M;
  ydif/=M;
  zdif/=M;

  /* These are the values in the matrix. */

  rxx=xdif*xdif + (1-xdif*xdif)*costheta;
  rxy=xdif*ydif*(1-costheta)-zdif*sintheta;
  rxz=xdif*zdif*(1-costheta)+ydif*sintheta;

  ryx=xdif*ydif*(1-costheta)+zdif*sintheta;
  ryy=ydif*ydif + (1-ydif*ydif)*costheta;
  ryz=ydif*zdif*(1-costheta)-xdif*sintheta;

  rzx=xdif*zdif*(1-costheta)-ydif*sintheta;
  rzy=ydif*zdif*(1-costheta)+xdif*sintheta;
  rzz=zdif*zdif + (1-zdif*zdif)*costheta;

  trans(thepoly, -one.xcord, -one.ycord, -one.zcord);

  /* Start pointers off in correct location. */
  pointx=&( POINTx(0,thepoly) );
  pointy=&( POINTy(0,thepoly) );
  pointz=&( POINTz(0,thepoly) );

  for( i=0; i<thepoly->numpoints; i++)
  {
    x=*pointx;
    y=*pointy;
    z=*pointz;
    *pointx= x * rxx + y * rxy + z * rxz;
    *pointy= x * ryx + y * ryy + z * ryz;
    *pointz= x * rzx + y * rzy + z * rzz;
    pointx+=3;
    pointy+=3;
    pointz+=3;
  }

  trans(thepoly, one.xcord, one.ycord, one.zcord);

}

/*****************************************************************************
3D viewing transformations
*****************************************************************************/

/* This sets up the parallel viewing transformation. */
void setparaview(double l, double theta)
{
  paratheta=theta;
  paral=l;
  xadj=paral*cos(paratheta*PI/180.0);
  yadj=paral*sin(paratheta*PI/180.0);
}

/* This function performs the parallel viewing transformation.  It is never
called directly but the calls are made via function pointer "viewtran". */
void parallel(MYPOLY *thepoly)
{
  byte i;
  double *xpoint,*ypoint,*zpoint;

  xpoint=&(thepoly->point[0].xcord);
  ypoint=&(thepoly->point[0].ycord);
  zpoint=&(thepoly->point[0].zcord);

  for( i=0; i<thepoly->numpoints ; i++)
  {
    *xpoint+= xadj * (*zpoint);
    *ypoint+= yadj * (*zpoint);
    xpoint+=3;
    ypoint+=3;
    zpoint+=3;
  }
}

/* This perspective viewing transformation.  The function pointer "viewtran"
will point to either perspect or parallel. */
void perspect(MYPOLY *thepoly)
{
  byte i;
  double thediv,*xpoint,*ypoint,*zpoint;

  xpoint=&(thepoly->point[0].xcord);
  ypoint=&(thepoly->point[0].ycord);
  zpoint=&(thepoly->point[0].zcord);

  for( i=0; i<thepoly->numpoints ; i++)
  {
    thediv=(*zpoint/dist + 1 );
    *xpoint/= thediv;
    *ypoint/= thediv;
    xpoint+=3;
    ypoint+=3;
    zpoint+=3;
  }
}

/*****************************************************************************
Object drawing and setup routines
*****************************************************************************/

void setupvideo(void)
{
  double scale=50.0; /* This is a world to viewing coordinate scale factor.*/
  struct viewporttype vp;

  getviewsettings(&vp);  /* Turbo C library function. */

  /* Put origin at center of the screen. */
  x_origin_map=(vp.right-vp.left)/2;
  y_origin_map=-(vp.top-vp.bottom)/2;

  x_scale_map=scale;
  y_scale_map=-scale*AspectRatio;

  /* Set up constants for both viewing transformations. */
  setparaview(0.5, 20.0);
  dist=6;

  /* Initial viewing transformation is perspective. */
  viewtran=perspect;

}


/* These macros are used in the polygon display routines. */
#define LINE0x(i) POINTx(showit->line[i].start,showit)
#define LINE0y(i) POINTy(showit->line[i].start,showit)
#define LINE0z(i) POINTz(showit->line[i].start,showit)

#define LINE1x(i) POINTx(showit->line[i].end,showit)
#define LINE1y(i) POINTy(showit->line[i].end,showit)
#define LINE1z(i) POINTz(showit->line[i].end,showit)

#define LINECOLOR(i) showit->line[i].color

void putpoly(MYPOLY *showit)
{
  byte i;

  for(i=0; i < showit->numlines; i++)
  {
    setcolor( LINECOLOR(i) );  /* Turbo C library function. */
    line                       /* Ditto */
    (
      x_origin_map + (int) ( LINE0x(i) * x_scale_map ),
      y_origin_map + (int) ( LINE0y(i) * y_scale_map ),
      x_origin_map + (int) ( LINE1x(i) * x_scale_map ),
      y_origin_map + (int) ( LINE1y(i) * y_scale_map )
    );
  }

}

/* It is faster to write a black polygon of over the old polygon than clear
the background screen.  This function deletes an old polygon.  The color
must be set to match the background. */
void delpoly(MYPOLY *showit)
{
  byte i;

  setcolor(0);

  for(i=0; i < showit->numlines; i++)
  {
    line
    (
      x_origin_map + (int) ( LINE0x(i) * x_scale_map ),
      y_origin_map + (int) ( LINE0y(i) * y_scale_map ),
      x_origin_map + (int) ( LINE1x(i) * x_scale_map ),
      y_origin_map + (int) ( LINE1y(i) * y_scale_map )
    );
  }

}

void tick(int ticks)
{
  static unsigned int far *timeptr = MK_FP(0,0x46C);
  static unsigned int lastime;

  while (ticks--)
  {
    while (*timeptr == lastime);
    lastime= *timeptr;
  }

}

void frames(boolean reset)
{
  static unsigned int far *timeptr = MK_FP(0,0x46C);
  static unsigned int lastime;
  static unsigned int framecount=0;

  if (reset)
  {
    lastime=*timeptr;
    framecount=0;
    return;
  }

  /* Check every 4 ticks */
  if ( 3 <  (*timeptr - lastime) )
  {
    frate=4.5 * framecount;

    if (autorate)
    {
      if (framecount>9) millis++;
      if ( (framecount<9) && (millis!=0) ) millis--;
    }

    framecount=0;
    lastime=*timeptr;
    return;
  }

  framecount++;

}

/* This function deletes the old polygon and puts the new one up.  The active
screens are toggled to do this smoothly. */
void redraw( MYPOLY *newpoly, MYPOLY *oldpoly)
{
  delpoly( oldpoly );

  putpoly( newpoly );

  frames(FALSE);

  setvisualpage( actpage );

  actpage=!actpage;

  setactivepage( actpage );

  /* Allow time for new page to be displayed before deleting it.  If this is
  not done then there will be a flickering due to the deleting of the polygon
  before the "setvisualpage" functions are complete. */
  delay(millis);

}

/* This similar to redraw except that the alternate screen is cleared.  This
is convenient when the old polygon is not known. */
void cleardraw( MYPOLY *newpoly)
{
  clearviewport();
  putpoly( newpoly );

  setvisualpage( actpage );

  actpage=!actpage;

  setactivepage( actpage );

  frames(TRUE);
}