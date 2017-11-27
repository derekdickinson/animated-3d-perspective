/* A convienenient macro to toggle a boolean value. */
#define NOT(x)    (x)^=0x01

/* Macros to access the proper fields of a data structure.  These allow
me to modify my data structure and not modify every reference to it. */
#define POLYIND(i) theobs[i].polyindex
#define ACTIVE(i)  theobs[i].active
#define SCAL(i)    theobs[i].scal
#define ROTX(i)    theobs[i].rotX
#define ROTY(i)    theobs[i].rotY
#define ROTZ(i)    theobs[i].rotZ
#define ROTREL(i)  theobs[i].rotrel
#define TRANS(i)   theobs[i].translat
#define SX(i)      theobs[i].sx
#define SY(i)      theobs[i].sy
#define SZ(i)      theobs[i].sz
#define AX(i)      theobs[i].aX
#define AY(i)      theobs[i].aY
#define AZ(i)      theobs[i].aZ
#define TX(i)      theobs[i].tX
#define TY(i)      theobs[i].tY
#define TZ(i)      theobs[i].tZ
#define AREL(i)    theobs[i].arel
#define P1(i)      theobs[i].p1
#define P2(i)      theobs[i].p2
