#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <sys/time.h>
#include <omp.h>

#define M_PI 3.14159265358979323846

/************************************* image variables ********************************************/
int pX, pY;
const int pXmax = 1280; // 2 billion+ px each side should be enough resolution right???????
const int pYmax = 1280; // for main antenna
const int iterationMax = 1000;
/**************************************************************************************************/

/****************************** coordinate plane to be rendered ***********************************/
const double CxMin = -2.2;
const double CxMax = 0.8;
const double CyMin = -1.5;
const double CyMax = 1.5;
/**************************************************************************************************/

/**************************************** file stuff **********************************************/
double pixelWidth; //=(CxMax-CxMin)/pXmax;
double pixelHeight; // =(CyMax-CyMin)/pYmax;
const int maxColorComponentValue = 255; // rgb - SDR colorspace (8 bits per color)
FILE * fp;
char * filename = "..\\..\\output\\mandelbrotC.ppm";
// char * comment = "# "; // comment should start with #
/**************************************************************************************************/

/************************************* render parameters ******************************************/
unsigned char stripeDensity = 7; // higher is more dense
int i_skip = 1; // exclude (i_skip+1) elements from average
const double escapeRadius = 1000000; // big! (bail-out value)
double lnER;
/**************************************************************************************************/

/**
 * Function: get_c
 * ---------------
 * Gets the corresponding coordinate point for the location of a pixel.
 *
 * Inputs:
 *  iX: number of x-iteration
 *  iY: number of y-iteration
 *
 * Returns:
 *  Complex double form of the coordinate.
 */
double _Complex get_c(int iX, int iY) {
    double Cx, Cy;
    Cy = CyMax - iY * pixelHeight;
    //if (fabs(Cy)< pixelHeight/3.0) Cy=0.0; // main antenna
    Cx = CxMin + iX * pixelWidth;
    return Cx + Cy * I;
}

/**
 * Function: c_dot
 * ---------------
 * Computes the dot product of 2 complex double vectors.
 *
 * Inputs:
 *  a: vector 1
 *  b: vector 2
 *
 * Returns:
 *  The dot product in double form.
 */
double c_dot(double _Complex a, double _Complex b) {
    return creal(a) * creal(b) + cimag(a) * cimag(b);
}

/**
 * Function: get_t
 * ---------------
 * Addend function used to get the value of A for stripe-average coloring method.
 * https://en.wikibooks.org/wiki/Fractals/Iterations_in_the_complex_plane/stripeAC
 *
 * Inputs:
 *  z: complex number
 *
 * Returns:
 *  Double number
 */
double getT(double _Complex z){
    return 0.5+0.5*sin(stripeDensity*carg(z));
}

/**
 * Function: colorize
 * ------------------
 * The bread and butter of this program; determines if point is within set by escape-time
 * algorithm and performs the colorization of the corresponding pixel.
 *
 * Inputs:
 *  c: the current coordinate point
 *  *row: the array of 1 row's pixel data
 *  iX: current pixel within the row
 *  iMax: maximum number of iterations
 *
 * Returns:
 *  0 if completed.
 */
int colorize(double _Complex c, unsigned char *row, int iX, int iMax) {
    /** global **/
    unsigned char b; // color
    int i; // iteration
    /** normal map **/
    double _Complex Z = 0.0; // initial value for iteration Z0
    double _Complex dC = 0.0; // derivative with respect to c
    double reflection = FP_ZERO; // inside
    double h2 = 1.5; // height factor of the incoming light
    double angle = 45.0 / 360.0; // incoming direction of light in turns (change 1st #)
    double _Complex v = cexp(2.0 * angle * M_PI * I); // unit 2D vector in this direction
    double _Complex u; // normal
    /** arg vars **/
    double A = 0.0; // A(n)
    double prevA = 0.0; // A(n-1)
    double R;
    double d; // smooth iteration count
    double de; // boundary descriptor


    /** do the compute **/
    for (i = 0; i < iMax; i++) {
        dC = 2.0 * dC * Z + 1.0;
        Z = Z * Z + c;
        if (i>i_skip) A += getT(Z);
        R = cabs(Z);
        /* shape checking algorithm
           skips iterating points within the main cardioid and secondary bulb, otherwise these
           would all hit the max iterations
           removes about 91% of the set from iteration
           REMOVE THIS IF NOT RENDERING THE ENTIRE SET - more computation per iteration if the main
           cardioid and secondary bulb are not shown onscreen */
        double q = ((creal(c) - 0.25) * (creal(c) - 0.25)) + (cimag(c) * cimag(c));
        double cardioid = 0.25 * cimag(c) * cimag(c);
        double bulb = 0.0625;
        if ((creal(c) * creal(c) + 2 * creal(c) + 1 + cimag(c) * cimag(c)) < bulb || (q * (q +
        (creal(c) - 0.25)) < cardioid)) {
            break;
        }
        /** get normal map **/
        if (R > escapeRadius) { // exterior of M set
            u = Z / dC;
            u = u / cabs(u);
            reflection = c_dot(u, v) + h2;
            reflection = reflection / (1.0 + h2); // rescale so that t does not get bigger than 1
            if (reflection < 0.0) reflection = 0.0;
            break;
        }
        prevA = A; // save value for interpolation
    }
    /** get striping **/
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

    /** assign pixel color values **/
    int subPixel = 3 * iX;
    if (reflection == FP_ZERO) { // interior of Mandelbrot set = black
        /* ppm files have pixels situated as groups of 3 ASCII chars in a row; the columns of the
           image file will be 3x as numerous as the rows
           attempting to store the image rows as a vector in memory and write to the file 1 row
           at a time */
        row[subPixel] = 0;
        row[subPixel+1] = 0;
        row[subPixel+2] = 0;
    }

    // exterior of Mandelbrot set -> normal
    else { // multiply the underlying stripe gradient by the reflectivity map
        if (A == FP_ZERO) b = 255; // boundary
        else b = (unsigned char) ((254-(100*A)) * reflection); // set color bounds for striping

        row[subPixel] = b;
        row[subPixel+1] = b;
        row[subPixel+2] = b;
    }
    return 0;
}

/**
 * Function: setup
 * ---------------
 * Sets up the .ppm file stream and parameters.
 *
 * Inputs:
 *  NULL
 *
 * Returns:
 *  NULL
 */
void setup() {
    pixelWidth = (CxMax - CxMin) / pXmax;
    pixelHeight = (CyMax - CyMin) / pYmax;
    lnER = log(escapeRadius);

    // create new ppm6 file, give it a name, and open it in binary mode
    fp = fopen(filename, "wb");
    // write ASCII header to the file
    fprintf(fp, "P6\n %d\n %d\n %d\n", pXmax, pYmax, maxColorComponentValue);
}

/**
 * Function: info
 * --------------
 * Provides debugging information from the file.
 *
 * Inputs:
 *  NULL
 *
 * Returns:
 *  NULL
 */
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

/**
 * Function: close
 * ---------------
 * Closes file stream and calls debugging info function.
 *
 * Inputs:
 *  NULL
 *
 * Returns:
 *  NULL
 */
void close() {
    fclose(fp);
    info();
}


/********************************************** main **********************************************/
int main() {
    struct timeval begin, end;
    gettimeofday(&begin, 0);

    double _Complex c;

    unsigned char row[pXmax * 3];

    setup();

    printf("Rendering row by row:\n");


    for (pY = 0; pY < pYmax; pY++)
    {
//#pragma omp parallel for schedule(dynamic)
        for (pX = 0; pX < pXmax; pX++)
        {
            // compute pixel coordinate
            c = get_c(pX, pY);
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
