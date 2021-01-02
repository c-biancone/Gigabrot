#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <time.h>
#include <omp.h>

#define M_PI 3.14159265358979323846 /* pi */


// image size
int iX, iY;
const int iXmax = 1000;
const int iYmax = 1000; // for main antenna

const int iterationMax = 1000;


// coordinate plane to be rendered
double Cx, Cy;
const double CxMin = -2.2;
const double CxMax = 0.8;
const double CyMin = -1.5;
const double CyMax = 1.5;

double pixelWidth; //=(CxMax-CxMin)/iXmax;
double pixelHeight; // =(CyMax-CyMin)/iYmax;

// rgb - SDR colorspace (8 bits per color)
const int maxColorComponentValue = 255;
FILE * fp;
char * filename = "mandelbrot.ppm"; // https://www.math.univ-toulouse.fr/~cheritat/wiki-draw/index

// char * comment = "# "; // comment should start with #

static unsigned char color[3]; // 24-bit rgb color
unsigned char s = 7; // stripe density

int i_skip = 1; // exclude (i_skip+1) elements from average
// bail-out value for the bailout test for escaping points radius of circle centered at the
// origin, exterior of such circle is a target set
const double escapeRadius = 1000000; // big !!!!
double lnER; // ln(ER)

double _Complex giveC(int iX, int iY) {
    double Cx, Cy;
    Cy = CyMax - iY * pixelHeight;
    //if (fabs(Cy)< pixelHeight/3.0) Cy=0.0; // main antenna
    Cx = CxMin + iX * pixelWidth;

    return Cx + Cy * I;
}

/*
 The dot product of two vectors a = [a1, a2, ..., an] and b = [b1, b2, ..., bn] is defined as:[1]
 d = a1b1 + a2b2
*/
double cdot(double _Complex a, double _Complex b) {
    return creal(a) * creal(b) + cimag(a) * cimag(b);
}

// the addend function
// input : complex number z
// output : double number t
double getT(double _Complex z){

    return 0.5+0.5*sin(s*carg(z));

}



int colorize(double _Complex c, unsigned char *color, int iMax) {

    // global
    unsigned char b; // color
    int i; // iteration
    // normal map vars
    double _Complex Z = 0.0; // initial value for iteration Z0
    double _Complex dC = 0.0; // derivative with respect to c
    double reflection = FP_ZERO; // inside
    double h2 = 1.5; // height factor of the incoming light
    double angle = 45.0 / 360.0; // incoming direction of light in turns
    double _Complex v = cexp(2.0 * angle * M_PI * I); // = exp(1j*angle*2*pi/360)  // unit 2D
        // vector in this direction
        // incoming light 3D vector = (v.re,v.im,h2)
    double _Complex u;
    //arg vars
    double A = 0.0; // A(n)
    double prevA = 0.0; // A(n-1)
    double R; // =radius = cabs(Z)
    double d; // smooth iteration count
    double de; // = 2 * z * log(cabs(z)) / dc;


// compute
    for (i = 0; i < iMax; i++) {
        dC = 2.0 * dC * Z + 1.0;
        Z = Z * Z + c;

        if (i>i_skip) A += getT(Z);

        R = cabs(Z);

        // shape checking algorithm
        // skips iterating points within the main cardioid and secondary bulb, otherwise these
        // would all hit the max iterations
        // removes about 91% of the set from iteration
        // REMOVE THIS IF NOT RENDERING THE ENTIRE SET - more computation per iteration if the main
        // cardioid and secondary bulb are not shown onscreen
        double q = ((creal(c) - 0.25) * (creal(c) - 0.25)) + (cimag(c) * cimag(c));
        double cardioid = 0.25 * cimag(c) * cimag(c);
        double bulb = 0.0625;
        if ((creal(c) * creal(c) + 2 * creal(c) + 1 + cimag(c) * cimag(c)) < bulb || (q * (q + (creal(c) - 0.25)) < cardioid)) {
            break;
        }


        if (R > escapeRadius) { // exterior of M set
            u = Z / dC;
            u = u / cabs(u);
            reflection = cdot(u, v) + h2;
            reflection = reflection / (1.0 + h2); // rescale so that t does not get bigger than 1
            if (reflection < 0.0) reflection = 0.0;
            break;
        }
        prevA = A; // save value for interpolation
    }
    if (i == iMax)
        A = -1.0; // interior
    else { // exterior
        de = 2 * R * log(R) / cabs(dC);
        if (de < (pixelWidth/3)) A = FP_ZERO; //  boundary
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

// assign
    if (reflection == FP_ZERO) { // interior of Mandelbrot set = inside_color = black
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
    }

    // exterior of Mandelbrot set = normal
    else { // multiply the underlying stripe gradient by the reflectivity map
        if (A == FP_ZERO) b = 255; // boundary
        else b = (unsigned char) ((254-(100*A)) * reflection);

        //b = (unsigned char) ((200-(100*arg)) * reflection);
        //b = (unsigned char) (255 - 255*potential );

        color[0] = b; // Red
        color[1] = b; // Green
        color[2] = b; // Blue
    }
    return 0;
}



void setup() {
    pixelWidth = (CxMax - CxMin) / iXmax;
    pixelHeight = (CyMax - CyMin) / iYmax;

    lnER = log(escapeRadius);

    // create new ppm6 file, give it a name, and open it in binary mode
    fp = fopen(filename, "wb");
    // write ASCII header to the file
    fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", "# no comment", iXmax, iYmax, maxColorComponentValue);
}

void info() {

    double distortion;
    // width/height
    double pixelsAspectRatio = (double) iXmax / iYmax;
    double worldAspectRatio = (CxMax - CxMin) / (CyMax - CyMin);
    printf("pixelsAspectRatio = %.16f \n", pixelsAspectRatio);
    printf("worldAspectRatio = %.16f \n", worldAspectRatio);
    distortion = pixelsAspectRatio - worldAspectRatio;
    printf("distortion = %.16f ( it should be zero !)\n", distortion);
    //printf("bailout value = Escape Radius = %.0f \n", escapeRadius);
    //printf("iterationMax = %d \n", iterationMax);
    //printf("i_skip = %d = number of skipped elements ( including t0 )= %d \n", i_skip, i_skip+1);
    printf("file %s saved.\n", filename);

}

void close() {
    fclose(fp);
    info();
}

// ************************************* main *************************
int main() {
    double execTime = 0.0;
    clock_t begin = clock();

    double _Complex c;

    setup();

    printf(" render = compute and write image data bytes to the file \n");
#pragma omp parallel for schedule(dynamic)
    for (iY = 0; iY < iYmax; iY++)
        for (iX = 0; iX < iXmax; iX++) {
            // compute pixel coordinate
            c = giveC(iX, iY);
            // compute  pixel color (24 bit = 3 bytes)
            colorize(c, color, iterationMax);
            // write color to the file
            fwrite(color, 1, 3, fp);
        }

    close();

    clock_t end = clock();
    execTime += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time elapsed: %f seconds", execTime);

    return 0;
}
