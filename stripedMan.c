/*
   c program: console, 1-file
   normal.c

   normal = Normal map effect  Mandelbrot set

   https://www.math.univ-toulouse.fr/~cheritat/wiki-draw/index.php/Mandelbrot_set
   thx for help:
   * 	Claude Heiland-Allen http://mathr.co.uk/blog/

   --------------------------------
   1. draws Mandelbrot set for Fc(z)=z*z +c
   using Mandelbrot algorithm ( boolean escape time )
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

 gcc normal.c -lm -Wall

 ./a.out
*/
#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <time.h>

#define M_PI 3.14159265358979323846 /* pi */

/* screen ( integer) coordinate */
int iX, iY;
const int iXmax = 1000;
const int iYmax = 1001; // for main antenna
/* world ( double) coordinate = parameter plane*/
double Cx, Cy;
const double CxMin = -2.2;
const double CxMax = 0.8;
const double CyMin = -1.5;
const double CyMax = 1.5;
/* */
double PixelWidth; //=(CxMax-CxMin)/iXmax;
double PixelHeight; // =(CyMax-CyMin)/iYmax;
/* color component ( R or G or B) is coded from 0 to 255 */
/* it is 24 bit color RGB file */
const int MaxColorComponentValue = 255;
FILE * fp;
char * filename = "mandelbrot.ppm"; // https://www.math.univ-toulouse.fr/~cheritat/wiki-draw/index
// .php/File:M-bump-1.png
char * comment = "# "; /* comment should start with # */

static unsigned char color[3]; // 24-bit rgb color

unsigned char s = 7; // stripe density

/*  */

const int IterationMax = 1000; //  N in wiki

int i_skip = 2; // exclude (i_skip+1) elements from average

/* bail-out value for the bailout test for escaping points
 radius of circle centered ad the origin, exterior of such circle is a target set  */
const double EscapeRadius = 1000000; // big !!!!

double lnER; // ln(ER)

double complex give_c(int iX, int iY) {
    double Cx, Cy;
    Cy = CyMax - iY * PixelHeight;
    //if (fabs(Cy)< PixelHeight/3.0) Cy=0.0; /* Main antenna */
    Cx = CxMin + iX * PixelWidth;

    return Cx + Cy * I;
}

/*
 The dot product of two vectors a = [a1, a2, ..., an] and b = [b1, b2, ..., bn] is defined as:[1]
 d = a1b1 + a2b2
*/
double cdot(double complex a, double complex b) {
    return creal(a) * creal(b) + cimag(a) * cimag(b);
}

// the addend function
// input : complex number z
// output : double number t
double Give_t(double complex z){

    return 0.5+0.5*sin(s*carg(z));

}

//
// output
//
double GiveReflection(double complex C, int iMax) {
    int i = 0; // iteration

    double complex Z = 0.0; // initial value for iteration Z0
    double complex dC = 0.0; // derivative with respect to c
    double reflection = FP_ZERO; // inside

    double h2 = 1.5; // height factor of the incoming light
    double angle = 45.0 / 360.0; // incoming direction of light in turns
    double complex v = cexp(2.0 * angle * M_PI * I); // = exp(1j*angle*2*pi/360)  // unit 2D vector in this direction
    // incoming light 3D vector = (v.re,v.im,h2)

    double complex u;

    for (i = 0; i < iMax; i++) {
        dC = 2.0 * dC * Z + 1.0;
        Z = Z * Z + C;

        if (cabs(Z) > EscapeRadius) { // exterior of M set
            u = Z / dC;
            u = u / cabs(u);
            reflection = cdot(u, v) + h2;
            reflection = reflection / (1.0 + h2); // rescale so that t does not get bigger than 1
            if (reflection < 0.0) reflection = 0.0;
            break;
        }
    }
    return reflection;
}

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

int compute_color(complex double c, unsigned char color[3]) {

double reflection;
double arg;
unsigned char b;

// compute
reflection = GiveReflection(c, IterationMax);
arg = Give_Arg(c, IterationMax);

if (reflection == FP_ZERO) { /*  interior of Mandelbrot set = inside_color = blue */
color[0] = 0; // M_waves
color[1] = 0;
color[2] = 0;
}

// exterior of Mandelbrot set = normal
else // multiply the underlying gradient by the reflectivity map
{

if (arg == FP_ZERO) b = 255; // boundary
else b = (unsigned char) ((254-(100*arg)) * reflection);

//b = (unsigned char) ((200-(100*arg)) * reflection);
//b = (unsigned char) (255 - 255*potential );

color[0] = b; /* Red*/
color[1] = b; /* Green */
color[2] = b; /* Blue */
}

return 0;
}

void setup() {
    PixelWidth = (CxMax - CxMin) / iXmax;
    PixelHeight = (CyMax - CyMin) / iYmax;

    lnER = log(EscapeRadius);

    /*create new file,give it a name and open it in binary mode  */
    fp = fopen(filename, "wb"); /* b -  binary mode */
    /*write ASCII header to the file*/
    fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, iXmax, iYmax, MaxColorComponentValue);
}

void info() {

    double distortion;
    // width/height
    double PixelsAspectRatio = (double) iXmax / iYmax; // https://en.wikipedia.org/wiki/Aspect_ratio_(image)
    double WorldAspectRatio = (CxMax - CxMin) / (CyMax - CyMin);
    printf("PixelsAspectRatio = %.16f \n", PixelsAspectRatio);
    printf("WorldAspectRatio = %.16f \n", WorldAspectRatio);
    distortion = PixelsAspectRatio - WorldAspectRatio;
    printf("distortion = %.16f ( it should be zero !)\n", distortion);
    printf("bailout value = Escape Radius = %.0f \n", EscapeRadius);
    printf("IterationMax = %d \n", IterationMax);
    printf("i_skip = %d = number of skipped elements ( including t0 )= %d \n", i_skip, i_skip+1);

    // file
    printf("file %s saved.\n", filename);

}

void close() {
    fclose(fp);
    info();
}

// ************************************* main *************************
int main() {
    complex double c;

    setup();

    printf(" render = compute and write image data bytes to the file \n");
#pragma omp parallel for schedule(static) private(iY, iX, c, color)

    for (iY = 0; iY < iYmax; iY++)
        for (iX = 0; iX < iXmax; iX++) { // compute pixel coordinate
            c = give_c(iX, iY);
            /* compute  pixel color (24 bit = 3 bytes) */
            compute_color(c, color);
            /*write color to the file*/
            fwrite(color, 1, 3, fp);
        }

    close();

    return 0;
}
