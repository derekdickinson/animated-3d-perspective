/*
These are some type definetions and defines that are used in project.c
*/

#define TRUE  1
#define FALSE 0

#define MAXPOINTS 40
#define MAXLINES  70

typedef unsigned char byte;

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
