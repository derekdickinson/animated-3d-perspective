/*
  Second part of CS-645 project by Derek Dickinson. Spring 1988
  (first part is project.exe)

  The file projstuf.obj is shared by both programs.

  This program differs from the first in that it allows multiple objects to
  be displayed but they are not in motion.  Time constraints would make it
  difficult to update too many objects and get the effect of animation.
*/
#include <graphics.h>
#include <bios.h>
#include <ctype.h>
#include <conio.h>
#include <math.h>
#include <mem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Macros and defines are in this file. */
#include "macros.inc"

/* Typedefs are in here. */
#include "multyobj.inc"

/* Function prototypes are in here. */
#include "protos.inc"

/* These externs are from projstuf.c */
extern double AspectRatio;
extern int texheight;

/* Pointer to current viewing transformation. */
void (near *viewtran)(MYPOLY *thepoly);

/* These values define the mapping from world coordinate to viewing
coordinates. */
int x_origin_map,y_origin_map;
double x_scale_map,y_scale_map;

/* These variables are used for the 3D to 2D viewing transformations.*/
double xadj,yadj,paral,paratheta,dist;
boolean persp=TRUE;

/* These define the current active page. */
int actpage=1;

/* An array of polygons is declared in this file. */
#include "thepolys.inc"

/* Number of polygons supported. */
byte numpoly=sizeof(thepolys)/sizeof(MYPOLY);

OBJECT theobs[10]=
{
  {
    0, /* index 0, the star */
    1, /* True, the poly is active */
    0,0,0,0,0,1, /* No transformations active yet. */
    1,1,1,0,0,0,0,-8,5,7, /* no change values if transformation is enabled. */
    { -1,-1,-1 },  /* Initial rotation line. */
    {  1, 1, 1 }
  },
  {
    1,
    1, /* True, the poly is active */
    0,0,0,0,0,1, /* No transformations active yet. */
    1,1,1,0,0,0,0,0,5,7, /* no change values if transformation is enabled. */
    { -1,-1,-1 },  /* Initial rotation line. */
    {  1, 1, 1 }
  },
  {
    2,
    1, /* True, the poly is active */
    0,0,0,0,0,1, /* No transformations active yet. */
    1,1,1,0,0,0,0,8,5,7, /* no change values if transformation is enabled. */
    { -1,-1,-1 },  /* Initial rotation line. */
    {  1, 1, 1 }
  },
  {
    3,
    1, /* True, the poly is active */
    0,0,0,0,0,1, /* No transformations active yet. */
    1,1,1,0,0,0,0,-8,0,7, /* no change values if transformation is enabled. */
    { -1,-1,-1 },  /* Initial rotation line. */
    {  1, 1, 1 }
  },
  {
    4,
    1, /* True, the poly is active */
    0,0,0,0,0,1, /* No transformations active yet. */
    1,1,1,0,0,0,0,0,0,7, /* no change values if transformation is enabled. */
    { -1,-1,-1 },  /* Initial rotation line. */
    {  1, 1, 1 }
  },
  {
    5,
    1, /* True, the poly is active */
    0,0,0,0,0,1, /* No transformations active yet. */
    1,1,1,0,0,0,0,8,0,7, /* no change values if transformation is enabled. */
    { -1,-1,-1 },  /* Initial rotation line. */
    {  1, 1, 1 }
  },
  {
    6,
    1, /* True, the poly is active */
    0,0,0,0,0,1, /* No transformations active yet. */
    1,1,1,0,0,0,0,-8,-5,7, /* no change values if transformation is enabled. */
    { -1,-1,-1 },  /* Initial rotation line. */
    {  1, 1, 1 }
  },
  {
    7,
    1, /* True, the poly is active */
    0,0,0,0,0,1, /* No transformations active yet. */
    1,1,1,0,0,0,0,0,-5,7, /* no change values if transformation is enabled. */
    { -1,-1,-1 },  /* Initial rotation line. */
    {  1, 1, 1 }
  },
  {
    8,
    1, /* True, the poly is active */
    0,0,0,0,0,1, /* No transformations active yet. */
    1,1,1,0,0,0,0,8,-5,7, /* no change values if transformation is enabled. */
    { -1,-1,-1 },  /* Initial rotation line. */
    {  1, 1, 1 }
  },
};

/* Number of objects. */
byte numobs=10;

/* Index of currently active object. */
int objind;

/* Value which determines the type of change which certain keys will cause. */
int changtype=0;

/* The current delta value. */
double curdel=0.5;

void main()
{
  Initialize();			/* Set system into Graphics mode     */

  setupvideo();

  statdemo3D();

  closegraph();			/* Return the system to text mode	*/
}

/*****************************************************************************
Static interactive stuff.
*****************************************************************************/

void statdemo3D(void)
{
  boolean done;

  cleartextport();
  clearviewport();

  for ( done=FALSE; !done;)
  {
    dispstat();
    dispobs();
    while ( !bioskey(1) );
    done=usekey();
  }

}

void dispobs(void)
{
  byte i;
  MYPOLY thepoly;

  actpage=!actpage;

  for(i=0; i<numobs; i++)
  {
    if (ACTIVE(i))
    {
      thepoly=thepolys[POLYIND(i)];
      if ( SCAL(i)   ) scale( &thepoly, SX(i), SY(i), SZ(i) );
      if ( ROTX(i)   ) rotateaxis( &thepoly, AX(i), XAXIS );
      if ( ROTY(i)   ) rotateaxis( &thepoly, AY(i), YAXIS );
      if ( ROTZ(i)   ) rotateaxis( &thepoly, AZ(i), ZAXIS );
      if ( ROTREL(i) ) rotateline( &thepoly, AREL(i), P1(i), P2(i) );
      if ( TRANS(i)  ) trans( &thepoly, TX(i), TY(i), TZ(i) );
      viewtran( &thepoly );
      putpoly( &thepoly );
    }
  }

  setvisualpage( actpage );

  setactivepage( !actpage );
  clearviewport();
}

boolean usekey(void)
{

  switch ( bioskey(1) & 0x00FF)
  {
    case 0x1b: case 'Q': bioskey(0); return(TRUE);

    /* Toggle transformations on and off. */
    case 'A': case 'a':
    case  1 : NOT(ACTIVE(objind));  break; /* Ctrl-A */
    case 18 : NOT(ROTREL(objind));  break; /* Ctrl-R */
    case 'S': case 's':
    case 19 : NOT(SCAL(objind));    break; /* Ctrl-S */
    case 'T': case 't':
    case 20 : NOT(TRANS(objind));   break; /* Ctrl-T */
    case 24 : NOT(ROTX(objind));    break; /* Ctrl-X */
    case 25 : NOT(ROTY(objind));    break; /* Ctrl-Y */
    case 26 : NOT(ROTZ(objind));    break; /* Ctrl-Z */

    case 'R': AREL(objind)+=curdel; break;
    case 'r': AREL(objind)-=curdel; break;

    case 'X': case 'x': changx(); break;
    case 'Y': case 'y': changy(); break;
    case 'Z': case 'z': changz(); break;

    case 'O': objind=( ++objind % numobs ); break;
    case 'o': objind=(objind==0) ? numobs-1 : objind-1; break;
    case 'P': POLYIND(objind)=( ++POLYIND(objind) % numpoly ); break;
    case 'p': POLYIND(objind)=(POLYIND(objind)==0) ? numpoly-1 : POLYIND(objind)-1; break;

    case 'V': case 'v':
      viewtran=(persp) ? parallel : perspect;
      NOT(persp); break;
    case 'L': paral+=curdel; break;
    case 'l': paral-=curdel; break;
    case 'K': paratheta+=1.0; setparaview( paral, paratheta ); break;
    case 'k': paratheta-=1.0; setparaview( paral, paratheta ); break;
    case 'D': dist+=curdel;  break;
    case 'd': dist-=curdel;  break;

    /* Adjust Delta value, these keys were chosen due to convenient location
    on numeric keyboard.*/
    case '7': curdel+=1.00; break;
    case '8': curdel+=0.10; break;
    case '9': curdel+=0.01; break;
    case '1': curdel-=1.00; break;
    case '2': curdel-=0.10; break;
    case '3': curdel-=0.01; break;

    default:
      switch (bioskey(1))
      {
        case 0x4d00: case 0x4800:
          if (++changtype > 4) changtype=0;
          break;
        case 0x4b00: case 0x5000:
          if (--changtype < 0) changtype=4;
          break;
        case 0x1300: AREL(objind)=0;                     break; /* Alt-R */
        case 0x1F00: SX(objind)=SY(objind)=SZ(objind)=1; break; /* Alt-S */
        case 0x1400: TX(objind)=TY(objind)=TZ(objind)=0; break; /* Alt-T */
        case 0x2D00: AX(objind)=0;                       break; /* Alt-X */
        case 0x1500: AY(objind)=0;                       break; /* Alt-Y */
        case 0x2C00: AZ(objind)=0;                       break; /* Alt-Z */
      }
  }

  bioskey(0);
  return(FALSE);
}

void changx(void)
{
  if (islower(bioskey(1) & 0x00ff) ) curdel*=-1;
  switch (changtype)
  {
    case 0: AX(objind)+=curdel;        break;
    case 1: P1(objind).xcord+=curdel;  break;
    case 2: P2(objind).xcord+=curdel;  break;
    case 3: TX(objind)+=curdel;        break;
    case 4: SX(objind)+=curdel;        break;
  }
  if (islower(bioskey(1) & 0x00ff) ) curdel*=-1;
}

void changy(void)
{
  if (islower(bioskey(1) & 0x00ff) ) curdel*=-1;
  switch (changtype)
  {
    case 0: AY(objind)+=curdel;       break;
    case 1: P1(objind).ycord+=curdel; break;
    case 2: P2(objind).ycord+=curdel; break;
    case 3: TY(objind)+=curdel;       break;
    case 4: SY(objind)+=curdel;       break;
  }
  if (islower(bioskey(1) & 0x00ff) ) curdel*=-1;
}

void changz(void)
{
  if (islower(bioskey(1) & 0x00ff)) curdel*=-1;
  switch (changtype)
  {
    case 0: AZ(objind)+=curdel;       break;
    case 1: P1(objind).zcord+=curdel; break;
    case 2: P2(objind).zcord+=curdel; break;
    case 3: TZ(objind)+=curdel;       break;
    case 4: SZ(objind)+=curdel;       break;
  }
  if (islower(bioskey(1) & 0x00ff)) curdel*=-1;
}

#define STATSET( ind, bool, ch )  status[ind]= (bool) ? ch : ' '

void dispstat(void)
{
  char status[20];
  int x,y;

  x=10; y=4; cleartextport();

  switch (changtype)
  {
    case 0:
      gprintf2( &x, &y,"Spins ( %4.2f, %4.2f, %4.2f ) ",
      AX(objind), AY(objind), AZ(objind) );
      break;
    case 1:
      gprintf2( &x, &y,"Rotate [[ %4.2f, %4.2f, %4.2f ]] - ( %4.2f, %4.2f, %4.2f ), %4.2f",
        P1(objind).xcord, P1(objind).ycord, P1(objind).zcord,
        P2(objind).xcord, P2(objind).ycord, P2(objind).zcord, AREL(objind));
        break;
      case 2:
        gprintf2( &x, &y,"Rotate ( %4.2f, %4.2f, %4.2f ) - [[ %4.2f, %4.2f, %4.2f ]], %4.2f",
        P1(objind).xcord, P1(objind).ycord, P1(objind).zcord,
        P2(objind).xcord, P2(objind).ycord, P2(objind).zcord, AREL(objind));
        break;
      case 3:
        gprintf2( &x, &y,"Translate [[ %4.2f, %4.2f, %4.2f ]] ",
          TX(objind), TY(objind), TZ(objind));
        break;
      case 4:
        gprintf2( &x, &y,"Scaling [[ %4.2f, %4.2f, %4.2f ]] ",
          SX(objind), SY(objind), SZ(objind));
        break;
    }
    memset( status,0x20,sizeof(status));
    STATSET( 0, ACTIVE(objind), 'A'); STATSET( 2, SCAL(objind)  , 'S');
    STATSET( 4, ROTX(objind)  , 'X'); STATSET( 6, ROTY(objind)  , 'Y');
    STATSET( 8, ROTZ(objind)  , 'Z'); STATSET(10, ROTREL(objind), 'R');
    STATSET(12, TRANS(objind) , 'T'); status[13]=0;

    if (persp)
      gprintf2( &x, &y,
      "obj=%d | status = %s | del=%4.2f | Persp. dist=%4.2f",
       objind,       status,     curdel,              dist);
    else
      gprintf2( &x, &y,
      "obj=%d | status = %s | del=%4.2f | Para. len=%4.2f, ang=%4.2f ",
       objind,       status,     curdel,            paral,  paratheta);

}

/*****************************************************************************
scaling, translations and rotations. Identical to those of project.c.
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


void rotateaxis(MYPOLY *thepoly, double theta,byte axis )
{
  byte i;
  double *dim0ptr,*dim1ptr;
  double dim0,sintheta, costheta;

  theta=theta*PI/180.0;

  switch (axis)
  {
    case 0:
      dim0ptr=&(thepoly->point[0].ycord);
      dim1ptr=&(thepoly->point[0].zcord);
      break;
    case 1:
      dim0ptr=&(thepoly->point[0].xcord);
      dim1ptr=&(thepoly->point[0].zcord);
      theta*=-1;
      break;
    case 2:
      dim0ptr=&(thepoly->point[0].xcord);
      dim1ptr=&(thepoly->point[0].ycord);
      break;
  }
  sintheta=sin(theta);
  costheta=cos(theta);

  for( i=0; i<thepoly->numpoints; i++)
  {
    dim0=*dim0ptr;
    *dim0ptr=dim0 * costheta - *dim1ptr * sintheta;
    *dim1ptr=dim0 * sintheta + *dim1ptr * costheta;
    dim0ptr+=3;
    dim1ptr+=3;
  }

}

#define POINTx(i,poly) poly->point[i].xcord
#define POINTy(i,poly) poly->point[i].ycord
#define POINTz(i,poly) poly->point[i].zcord

void rotateline(MYPOLY *thepoly, double theta, POINT one, POINT two)
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
3D viewing transformations.  These are very similar routines to those of
project.c.  The difference is in the perspective routine.
*****************************************************************************/

void setparaview(double l, double theta)
{
  paratheta=theta;
  paral=l;
  xadj=paral*cos(paratheta*PI/180.0);
  yadj=paral*sin(paratheta*PI/180.0);
}

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

/* This routine was modified to perform better when some points cannot be
viewed.  "project.c" does not perform the extra work since it is slower to
do the extra checks. */
void perspect(MYPOLY *thepoly)
{
  byte i;
  double thediv,*xpoint,*ypoint,*zpoint;

  xpoint=&(thepoly->point[0].xcord);
  ypoint=&(thepoly->point[0].ycord);
  zpoint=&(thepoly->point[0].zcord);

  /* Check for strange value of dist. */
  if ( fabs(dist)<0.001 )
  {
    for( i=0; i<thepoly->numpoints ; i++)
    {
      *xpoint=TOOBIG+1;
      *ypoint=TOOBIG+1;
      xpoint+=3;
      ypoint+=3;
    }
  }

  for( i=0; i<thepoly->numpoints ; i++)
  {
    thediv=(*zpoint/dist + 1 );

    /* Eliminate points which cannot be plotted. */
    if ( fabs(thediv) < 0.001 )
    {
      *xpoint=TOOBIG+1;
      *ypoint=TOOBIG+1;
    }
    else
    {
      *xpoint/= thediv;
      *ypoint/= thediv;
    }

    xpoint+=3;
    ypoint+=3;
    zpoint+=3;
  }
}

/*****************************************************************************
Object drawing and setup routines.  These are identical to those of
project.c except that invalid lines are excluded.  See that source listing
for more comments.
*****************************************************************************/

void setupvideo(void)
{
  double scale=50.0;
  struct viewporttype vp;

  getviewsettings(&vp);

  x_origin_map=(vp.right-vp.left)/2;
  y_origin_map=-(vp.top-vp.bottom)/2;

  x_scale_map=scale;
  y_scale_map=-scale*AspectRatio;

  setparaview(0.5, 20.0);
  dist=10;
  viewtran=perspect;

}

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
    if (LINE0x(i)>=TOOBIG) continue; /* Check for invalid line. */
    if (LINE0y(i)>=TOOBIG) continue;
    if (LINE1x(i)>=TOOBIG) continue;
    if (LINE1y(i)>=TOOBIG) continue;
    setcolor( LINECOLOR(i) );
    line
    (
      x_origin_map + (int) ( LINE0x(i) * x_scale_map ),
      y_origin_map + (int) ( LINE0y(i) * y_scale_map ),
      x_origin_map + (int) ( LINE1x(i) * x_scale_map ),
      y_origin_map + (int) ( LINE1y(i) * y_scale_map )
    );
  }

}

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

void redraw( MYPOLY *newpoly, MYPOLY *oldpoly)
{
  actpage=!actpage;

  putpoly( newpoly );
  setvisualpage( actpage );

  setactivepage( !actpage );
  delpoly( oldpoly );
}

void cleardraw( MYPOLY *newpoly)
{
  actpage=!actpage;

  putpoly( newpoly );
  setvisualpage( actpage );

  setactivepage( !actpage );
  clearviewport();
}