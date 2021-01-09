#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <sys/time.h>
#include <omp.h>
#include <windows.h>

#define M_PI 3.14159265358979323846

#define HEADER_LEN 30 // 22 + length of resolution integers in characters
// eg. a 5000 x 5000 image is 30


// image size
int pX, pY;
const int pXmax = 500; // 2 billion+ px each side should be enough resolution right???????
const int pYmax = 500; // for main antenna

const int iterationMax = 1000;


// coordinate plane to be rendered
const double CxMin = -2.2;
const double CxMax = 0.8;
const double CyMin = -1.5;
const double CyMax = 1.5;

double pixelWidth; //=(CxMax-CxMin)/pXmax;
double pixelHeight; // =(CyMax-CyMin)/pYmax;
// rgb - SDR colorspace (8 bits per color)
const int maxColorComponentValue = 255;
FILE * fp;
char * filename = "mandelbrot.ppm";

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
double cDot(double _Complex a, double _Complex b) {
    return creal(a) * creal(b) + cimag(a) * cimag(b);
}

// the addend function
// input : complex number z
// output : double number t
double getT(double _Complex z){
    return 0.5+0.5*sin(s*carg(z));
}



int colorize(double _Complex c, unsigned char *row, int iX, int iMax) {

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

        // compute normal map
        if (R > escapeRadius) { // exterior of M set
            u = Z / dC;
            u = u / cabs(u);
            reflection = cDot(u, v) + h2;
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
        int thin = 3; // thinness of the border
        if (de < (pixelWidth / thin)) A = FP_ZERO; //  boundary
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
    int subPixel = 3 * iX;
    if (reflection == FP_ZERO) { // interior of Mandelbrot set = inside_color = black
        /*color[0] = 0;
        color[1] = 0;
        color[2] = 0;*/
        // ppm files have pixels situated as groups of 3 ASCII chars in a row; the columns of the image file will be 3x as numerous as the rows
        // attempting to store the image rows as a vector in memory and write to the file 1 row
        // at a time
        row[subPixel] = 0;
        row[subPixel+1] = 0;
        row[subPixel+2] = 0;
    }

        // exterior of Mandelbrot set = normal
    else { // multiply the underlying stripe gradient by the reflectivity map
        if (A == FP_ZERO) b = 255; // boundary
        else b = (unsigned char) ((254-(100*A)) * reflection);

        //b = (unsigned char) ((200-(100*arg)) * reflection);
        //b = (unsigned char) (255 - 255*potential );

        //color[0] = b; // Red
        //color[1] = b; // Green
        //color[2] = b; // Blue

        row[subPixel] = b;
        row[subPixel+1] = b;
        row[subPixel+2] = b;
    }
    return 0;
}



void setup() {
    pixelWidth = (CxMax - CxMin) / pXmax;
    pixelHeight = (CyMax - CyMin) / pYmax;

    lnER = log(escapeRadius);

    // create new ppm6 file, give it a name, and open it in binary mode
    fp = fopen(filename, "wb");
    // write ASCII header to the file
    fprintf(fp, "P6\n %d\n %d\n %d\n", pXmax, pYmax, maxColorComponentValue);


    /*
    unsigned char buf[HEADER_LEN + 3UL * pXmax * pYmax];
    sprintf((char *)buf, "P6 %d %d %d\n", pXmax, pYmax, maxColorComponentValue);
    unsigned char *p = buf + HEADER_LEN; */
}

void info() {

    double distortion;
    // width/height
    double pixelsAspectRatio = (double) pXmax / pYmax;
    double worldAspectRatio = (CxMax - CxMin) / (CyMax - CyMin);
    // printf("pixelsAspectRatio = %.16f \n", pixelsAspectRatio);
    // printf("worldAspectRatio = %.16f \n", worldAspectRatio);
    distortion = pixelsAspectRatio - worldAspectRatio;
    printf("distortion = %.16f (should be zero!)\n", distortion);
    // printf("bailout value = Escape Radius = %.0f \n", escapeRadius);
    // printf("iterationMax = %d \n", iterationMax);
    // printf("i_skip = %d = number of skipped elements ( including t0 )= %d \n", i_skip, i_skip+1);
    printf("file %s saved.\n", filename);

}

void close() {
    fclose(fp);
    info();
}


// ************************************* main **************************************
int main() {
    struct timeval begin, end;
    gettimeofday(&begin, 0);

    double _Complex c;

    // int pixels = 5000 * 3;
    unsigned char row[pXmax * 3];

    // typedef unsigned char pixel_t[3]; // array for the rgb values of each pixel in one row
    // allocate enough space in memory to hold the rgb values of one row of the image
    // pixel_t *pixels = malloc(sizeof(pixel_t) * pXmax);

    setup();

    printf("Rendering row by row:\n");


    for (pY = 0; pY < pYmax; pY++)
    {
#pragma omp parallel for schedule(dynamic)
        for (pX = 0; pX < pXmax; pX++)
        {
            // compute pixel coordinate
            c = giveC(pX, pY);
            // compute  pixel color (24 bit = 3 bytes)
            colorize(c, row, pX, iterationMax);
        }
        // write the cached row of pixels
        fwrite(row, 1, (size_t)sizeof(row), fp);
        //fwrite("\n", 1, 1, fp); // unnecessary
    }

    close();

    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double execTime = seconds + microseconds*1e-6;
    printf("Time elapsed: %f seconds", execTime);

    return 0;
}
