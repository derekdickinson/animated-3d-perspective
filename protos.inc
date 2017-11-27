void changx(void);
void changy(void);
void changz(void);

void statdemo3D(void);
void dispstat(void);
boolean usekey(void);
void dispobs(void);

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
