
#define TRUE  1
#define FALSE 0

#define TOOBIG 1000000L

#define XAXIS  0
#define YAXIS  1
#define ZAXIS  2

#define PI     3.14159265358979323846

#define MAXPOINTS 40
#define MAXLINES  70

/*
BLACK     BLUE          GREEN       CYAN
RED       MAGENTA       BROWN       LIGHTGRAY
DARKGRAY  LIGHTBLUE     LIGHTGREEN  LIGHTCYAN
LIGHTRED  LIGHTMAGENTA  YELLOW      WHITE
*/

#define L1       LIGHTCYAN
#define L2       LIGHTGREEN
#define L3       LIGHTRED
#define S1       LIGHTGRAY
#define S2       LIGHTGRAY

#define TOPCOL   LIGHTCYAN
#define SIDCOL   LIGHTRED
#define BOTCOL   LIGHTGREEN



typedef unsigned char byte;
typedef unsigned char boolean;

typedef struct
{
  double xcord,ycord,zcord;
} POINT;

typedef struct
{
  int start,end,color;
} LINE;

typedef struct
{
  int numpoints, numlines;
  POINT point[MAXPOINTS];
  LINE line[MAXLINES];
} MYPOLY;

typedef struct
{
  int polyindex;
  boolean active,scal,rotX,rotY,rotZ,rotrel,translat;
  float sx,sy,sz,aX,aY,aZ,arel,tX,tY,tZ;
  POINT p1,p2;
} OBJECT;

void Initialize(void);
int  gprintf(int *xloc, int *yloc, char *fmt, ... );
int  gprintf2(int *xloc, int *yloc, char *fmt, ... );
void cleartextport(void);

void putpoly( MYPOLY *showit );
