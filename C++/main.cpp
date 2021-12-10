#include "PGM.h"

#include <cmath>
#include <complex>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <omp.h>
#include <vector>

using namespace std;

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
//char * filename = "..\\..\\output\\mandelbrot.ppm";
string fileName = "..\\..\\output\\mandelbrot.ppm";
// char * comment = "# "; // comment should start with #
/**************************************************************************************************/

/************************************* render parameters ******************************************/
unsigned char stripeDensity = 7; // higher is more dense
int i_skip = 1; // exclude (i_skip+1) elements from average
const double escapeRadius = 1000000; // big! (bail-out value)
double lnER = log(escapeRadius);
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
complex<double> get_c(int iX, int iY) {
    double Cx = CxMin + iX * pixelWidth;
    double Cy = CyMax - iY * pixelWidth;
    complex<double> c = Cx + Cy * 1i;
    return c;
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
double c_dot(complex<double> a, complex<double> b) {
    return real(a) * real(b) + imag(a) * imag(b);
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
double getT(complex<double> z){
    return 0.5+0.5*sin(stripeDensity*arg(z));
}

/**
 * Shape checking algorithm - determines if point is within main cardioid or secondary bulb
 * Removes about 91% of the set from being iterated
 * Should not be implemented for a render that does not include these parts, will add unnecessary
 * computing
 * @param c - complex number location
 * @param color - pixel value
 * @return TRUE if within the main shapes
 */
bool shape_check(complex<double> c, unsigned char color)
{
    double q = ((real(c) - 0.25) * (real(c) - 0.25)) + (imag(c) * imag(c));
    double cardioid = 0.25 * imag(c) * imag(c);
    double bulb = 0.0625;
    if ((real(c) * real(c) + 2 * real(c) + 1 + imag(c) * imag(c)) < bulb ||
        (q * (q +(real(c) - 0.25)) < cardioid))
    {
        color = 0; // black
        return true;
    } else {
        return false;
    }
}

double normal_map(complex<double> Z, complex<double> dC)
{
    double h2 = 1.5; // height factor of the incoming light
    double angle = 45.0 / 360.0; // incoming direction of light in turns (change 1st #)
    complex<double> u = Z / dC;
    u = u / abs(u); // normalize
    complex<double> v = exp(2.0 * angle * M_PI * 1i); // unit 2D vector in this direction
    double reflection = c_dot(u, v) + h2;
    reflection /= (1.0 + h2); // rescale so that t does not get bigger than 1
    if (reflection < 0.0) reflection = 0.0;
    return reflection;
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
int colorize(complex<double> c, vector<unsigned char>& row, int iX, int iMax) {
    /** global **/
    unsigned char b; // color
    int i; // iteration
    /** normal map **/
    complex<double> Z = 0.0; // initial value for iteration Z0
    complex<double> dC = 0.0; // derivative with respect to c
    /** arg vars **/
    double A = 0.0; // A(n)
    double prevA = 0.0; // A(n-1)
    double R;
    double d; // smooth iteration count
    double de; // boundary descriptor


    /** do the compute **/
    if (!shape_check(c, b))
    {
        for (i = 0; i < iMax; i++)
        {
            // bread and butter of Mandelbrot iteration
            dC = 2.0 * dC * Z + 1.0;
            Z = Z * Z + c;

            if (i > i_skip)
            {
                A += getT(Z);
            }
            R = abs(Z);

            // if the iterations determine c is outside the set
            if (R > escapeRadius)
            {
                break;
            }
            prevA = A; // save value for interpolation
        }
        /** get striping **/
        if (i == iMax)
            A = -1.0; // interior
        else
        { // exterior
            de = 2 * R * log(R) / abs(dC);
            int thin = 3; // thinness of the border
            if (de < (pixelWidth / thin)) A = FP_ZERO; //  boundary
            else
            {
                // computing interpolated average
                A /= (i - i_skip); // A(n)
                prevA /= (i - i_skip - 1); // A(n-1)
                // smooth iteration count
                d = i + 1 + log(lnER / log(R)) / M_LN2;
                d = d - (int) d; // only fractional part = interpolation coefficient
                // linear interpolation
                A = d * A + (1.0 - d) * prevA;
            }
        }
    }

    /** assign pixel color values **/
    int subPixel = 3 * iX;
    if (i == iMax) { // interior of Mandelbrot set = black
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
        else b = (unsigned char) ((254-(100*A)) * normal_map(Z, dC)); // set color bounds for
        // striping

        row[subPixel] = b;
        row[subPixel+1] = b;
        row[subPixel+2] = b;
    }
    return 0;
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
void info(int width, int height) {
    double distortion;
    // width/height
    double pixelsAspectRatio = (double) width / height;
    double worldAspectRatio = (CxMax - CxMin) / (CyMax - CyMin);
    // printf("pixelsAspectRatio = %.16f \n", pixelsAspectRatio);
    // printf("worldAspectRatio = %.16f \n", worldAspectRatio);
    distortion = pixelsAspectRatio - worldAspectRatio;
    cout << "Distortion = " << distortion << " should be 0!\n";
    // printf("bailout value = Escape Radius = %.0f \n", escapeRadius);
    // printf("iterationMax = %d \n", iterationMax);
    // printf("i_skip = %d = number of skipped elements ( including t0 )= %d \n", i_skip, i_skip+1);
    cout << "File " << fileName << " saved\n";

}

int get_dims(istream& in)
{
    int tmp;
    in >> tmp;
    return tmp;
}


/********************************************** main **********************************************/
int main() {

    cout << "Enter width and height in pixels: \n";
    const int width = get_dims(cin);
    const int height = get_dims(cin);

    pixelWidth = (CxMax - CxMin) / width;
    pixelHeight= (CyMax - CyMin) / height;

    auto begin = chrono::steady_clock::now();

    // set up image stream for writing
    PGM pgm(fileName, width, height);
    if (!pgm.init_stream())
    {
        cout << "Could not open ofstream for image";
    }
    pgm.write_header();

    complex<double> c;
    vector<unsigned char> row{}; // didn't realize array needs compile-time const length :(
    row.resize(width * 3);

    //setup();

    cout << "Rendering row by row:\n";

    for (pY = 0; pY < height; pY++)
    {
//#pragma omp parallel for schedule(dynamic)
        for (pX = 0; pX < width; pX++)
        {
            // compute pixel coordinate
            c = get_c(pX, pY);
            // compute  pixel color (24 bit = 3 bytes)
            colorize(c, row, pX, iterationMax);
        }
        // write the cached row of pixels
        // implemented due to possibility of having huge image
        pgm.write_row(row);
    }

    pgm.close();

    info(width, height);

    auto end = chrono::steady_clock::now();
    cout << "Time elapsed:"
        << chrono::duration_cast<chrono::seconds>(end - begin).count()
        << " sec\n";
    return 0;
}
