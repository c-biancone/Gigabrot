/*
   c program: console, 1-file

   algorithms:
   - escape time
   - DEM/M = distance estimation
   - SAC/M = SAM/M

   samm = Stripe Average Method  =
   Stripe Average coloring = sac
   with :
   - skipping first (i_skip+1) points from average
   - linear interpolation

   https://en.wikibooks.org/wiki/Fractals/Iterations_in_the_complex_plane/stripeAC



   --------------------------------
   1. draws Mandelbrot set for complex quadratic polynomial
   Fc(z)=z*z +c
   using samm = Stripe Average Method/Coloring
   -------------------------------
   2. technique of creating ppm file is  based on the code of Claudio Rocchini
   http://en.wikipedia.org/wiki/Image:Color_complex_plot.jpg
   create 24 bit color graphic file ,  portable pixmap file = PPM
   see http://en.wikipedia.org/wiki/Portable_pixmap
   to see the file use external application ( graphic viewer)
   -----
   it is example  for :
   https://www.math.univ-toulouse.fr/~cheritat/wiki-draw/index.php/Mandelbrot_set

   -------------
   compile :



   gcc samm.c -lm -Wall


   ./a.out

   convert samm.ppm -resize 800x800 samm.png

   -------- git --------
*/
#include <stdio.h>
#include <math.h>
#include <complex.h> // https://stackoverflow.com/questions/6418807/how-to-work-with-complex-numbers-in-c
#include <omp.h> //OpenM


#define M_PI        3.14159265358979323846    /* pi */

/* screen ( integer) coordinate */
int iX,iY;
const int iXmax = 500;
const int iYmax = 500;

/* world ( double) coordinate = parameter plane*/
double Cx,Cy;

// c = -0.355298979690605  +1.178016112830515 i    period = 0
// c = 0.213487557597331  +0.604701020309395 i    period = 0
const double CxMin=-2.25;
const double CxMax=0.75;
const double CyMin=1.5;
const double CyMax= -1.5;
/* */
double PixelWidth; //=(CxMax-CxMin)/iXmax;
double PixelHeight; // =(CyMax-CyMin)/iYmax;

/* color component ( R or G or B) is coded from 0 to 255 */
/* it is 24 bit color RGB file */
const int MaxColorComponentValue=255;
FILE * fp;
char *filename="samm.ppm"; // https://commons.wikimedia.org/wiki/File:Mandelbrot_set_-_Stripe_Average_Coloring.png
char *comment="# ";/* comment should start with # */

static unsigned char color[3]; // 24-bit rgb color

unsigned char s = 7; // stripe density

const int IterationMax=2500; //  N in wiki
int i_skip = 2; // exclude (i_skip+1) elements from average

/* bail-out value for the bailout test for exaping points
   radius of circle centered ad the origin, exterior of such circle is a target set  */
const double EscapeRadius=1000000; // = ER big !!!!
double lnER; // ln(ER)




double complex give_c(int iX, int iY){
    double Cx,Cy;
    Cy=CyMax - iY*PixelHeight;
    //if (fabs(Cy)< PixelHeight/3.0) Cy=0.0; /* Main antenna */
    Cx=CxMin + iX*PixelWidth;
    //printf("assigned Cy and Cx\n");

    return Cx+Cy*I;


}



// the addend function
// input : complex number z
// output : double number t
double Give_t(double complex z){

    return 0.5+0.5*sin(s*carg(z));

}

/*
  input :
  - complex number
  - intege
  output = average or other estimators, like distance or interior

*/
double Give_Arg(double complex C , int iMax)
{
    int i=0; // iteration


    double complex Z= 0.0; // initial value for iteration Z0
    double A = 0.0; // A(n)
    double prevA = 0.0; // A(n-1)
    double R; // =radius = cabs(Z)
    double d; // smooth iteration count
    double complex dC = 0; // derivative
    double de; // = 2 * z * log(cabs(z)) / dc;


    // iteration = computing the orbit
    for(i=0;i<iMax;i++)
    {

        dC = 2 * Z * dC + 1;
        Z=Z*Z+C; // https://en.wikibooks.org/wiki/Fractals/Iterations_in_the_complex_plane/qpolynomials

        if (i>i_skip) A += Give_t(Z); //

        R = cabs(Z);
        if(R > EscapeRadius) break; // exterior of M set

        prevA = A; // save value for interpolation

    } // for(i=0


    if (i == iMax)
        A = -1.0; // interior
    else { // exterior
        de = 2 * R * log(R) / cabs(dC);
        if (de < PixelWidth) A = FP_ZERO; //  boundary
        else {
            // computing interpolated average
            A /= (i - i_skip) ; // A(n)
            prevA /= (i - i_skip - 1) ; // A(n-1)
            // smooth iteration count
            d = i + 1 + log(lnER/log(R))/M_LN2;
            d = d - (int)d; // only fractional part = interpolation coefficient
            // linear interpolation
            A = d*A + (1.0-d)*prevA;
        }
    }

    return A;
}


/*

 input = complex number
 output
  - color array as Formal parameters
  - int = return code
*/
int compute_color(complex double c, unsigned char color[3]){

double arg;
unsigned char r;
unsigned char g;
unsigned char b;

// compute
arg = Give_Arg( c, IterationMax);

//
if (arg < 0.0)
{
/*  interior of Mandelbrot set = inside_color =  */
r = 78;
g = 93;
b = 148;
}

else // exterior of Mandelbrot set = CPM --- gray gradient
{

if (arg == FP_ZERO) r = g = b = 100; // boundary
else r = g = b = (unsigned char) (200 - 100*arg );


};

color[0]= r;  /* Red*/
color[1]= g;  /* Green */
color[2]= b;  /* Blue */
//printf("colors computed\n");
return 0;
}



void setup(){

    //
    PixelWidth=(CxMax-CxMin)/iXmax;
    PixelHeight=(CyMax-CyMin)/iYmax;

    lnER = log(EscapeRadius); // ln(ER)


    /*create new file,give it a name and open it in binary mode  */
    fp= fopen(filename,"wb"); /* b -  binary mode */
    printf("file opened\n");
    /*write ASCII header to the file*/
    fprintf(fp,"P6\n %s\n %d\n %d\n %d\n",comment,iXmax,iYmax,MaxColorComponentValue);
    printf("wrote ascii header to the file\n");

}




void info(){

    double distortion;
    // widt/height
    double PixelsAspectRatio = (double)iXmax/iYmax;  // https://en.wikipedia.org/wiki/Aspect_ratio_(image)
    double WorldAspectRatio = (CxMax-CxMin)/(CyMax-CyMin);
    printf("PixelsAspectRatio = %.16f \n", PixelsAspectRatio );
    printf("WorldAspectRatio = %.16f \n", WorldAspectRatio );
    distortion = PixelsAspectRatio - WorldAspectRatio;
    printf("distortion = %.16f ( it should be zero !)\n", distortion );
    printf("\n");
    printf("bailout value = Escape Radius = %.0f \n", EscapeRadius);
    printf("IterationMax = %d \n", IterationMax);
    printf("i_skip = %d = number of skipped elements ( including t0 )= %d \n", i_skip, i_skip+1);

    // file
    printf("file %s saved. It is called .... in  A Cheritat wiki\n", filename);

}





void close(){

    fclose(fp);
    printf("file closed\n");
    info();


}





// ************************************* main *************************
int main()
{

    complex double c;



    setup();


    printf(" render = compute and write image data bytes to the file \n");
    //#pragma omp parallel for schedule(static) private(iY, iX, c, color)

    for(iY=0;iY<iYmax;iY++)
    {
        for(iX=0;iX<iXmax;iX++)
        { // compute pixel coordinate
            c = give_c(iX, iY);
            /* compute  pixel color (24 bit = 3 bytes) */
            compute_color(c,color);
            /*write color to the file*/
            fwrite(color,1,3,fp);
        }
        printf("%d\n", iY);
    }

    printf("file data written\n");


    close();


    return 0;
}