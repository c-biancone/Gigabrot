
#include "cuda.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <complex>
#include <cuComplex.h>
#include <math.h>

#define M_PI  3.14159265358979323846
#define M_LN2 0.69314718055994530942

// CUDA error check
#define cuda_check(call) \
	{\
	cudaError_t res = (call);\
	if(res != cudaSuccess) {\
		const char* err_str = cudaGetErrorString(res);\
		fprintf(stderr, "%s (%d): %s in %s", __FILE__, __LINE__, err_str, #call);	\
		exit(-1);\
		}\
	}

/************************************* image variables ********************************************/
int pX, pY;
const int pXmax = 500; // 2 billion+ px each side should be enough resolution right???????
const int pYmax = 500; // for main antenna
const int iterationMax = 1000;
#define BS 256
/**************************************************************************************************/

/****************************** coordinate plane to be rendered ***********************************/
const double CxMin = -2.2;
const double CxMax = 0.8;
const double CyMin = -1.5;
const double CyMax = 1.5;
/**************************************************************************************************/

/**************************************** file stuff **********************************************/
double pixelWidth; // =(CxMax-CxMin)/pXmax;
double pixelHeight; // =(CyMax-CyMin)/pYmax;
const int maxColorComponentValue = 255; // rgb - SDR colorspace (8 bits per color)
FILE* fp;
char* filename = "mandelbrot.ppm";
// char * comment = "# "; // comment should start with #
/**************************************************************************************************/

/************************************* render parameters ******************************************/
unsigned char stripeDensity = 7; // higher is more dense
int i_skip = 1; // exclude (i_skip+1) elements from average
const double escapeRadius = 1000000; // big! (bail-out value)
double lnER;
/**************************************************************************************************/

double gpuTime = 0;

// number of threads PER ROW
int threadNum = pXmax;

/************************************ define complex type *****************************************/
struct _Complex {
	__host__ __device__ _Complex(float re, float im = 0) {
		this->re = re;
		this->im = im;
	}
	/** real and imaginary part */
	double re, im;
}; // struct complex
/********************************* define complex operators ***************************************/
inline __host__ __device__ _Complex operator+
(const _Complex& a, const _Complex& b) {
	return _Complex(a.re + b.re, a.im + b.im);
}
inline __host__ __device__ _Complex operator-
(const _Complex& a) {
	return _Complex(-a.re, -a.im);
}
inline __host__ __device__ _Complex operator-
(const _Complex& a, const _Complex& b) {
	return _Complex(a.re - b.re, a.im - b.im);
}
inline __host__ __device__ _Complex operator*
(const _Complex& a, const _Complex& b) {
	return _Complex(a.re * b.re - a.im * b.im, a.im * b.re + a.re * b.im);
}
inline __host__ __device__ float abs2(const _Complex& a) {
	return a.re * a.re + a.im * a.im;
}
inline __host__ __device__ _Complex operator/
(const _Complex& a, const _Complex& b) {
	float invabs2 = 1 / abs2(b);
	return _Complex((a.re * b.re + a.im * b.im) * invabs2,
		(a.im * b.re - b.im * a.re) * invabs2);
}  // operator/
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

_Complex get_c(int iX, int iY) {
	double Cx, Cy;
	Cy = CyMax - iY * pixelHeight;
	//if (fabs(Cy)< pixelHeight/3.0) Cy=0.0; // main antenna
	Cx = CxMin + iX * pixelWidth;
	return (Cx, Cy);
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
double c_dot(_Complex a, _Complex b) {
	return a.re * b.re + a.im * b.im;
}

/**
 * Function: c_arg
 * ---------------
 * Computes the argument of a given complex vector.
 *
 * Inputs:
 *  a: vector 1
 *
 * Returns:
 *  The argument in double form.
 */
double c_arg(_Complex a) {
	return atan2(a.im, a.re);
}
/**
 * Function: c_abs
 * ---------------
 * Computes the magnitude of a given vector.
 *
 * Inputs:
 *  a: vector 1
 *
 * Returns:
 *  The magnitude in double form.
 */
double c_abs(_Complex a) {
    return sqrt((a.re * a.re) + (a.im * a.im));
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
double getT(_Complex z) {
	return 0.5 + 0.5 * sin(stripeDensity * c_arg(z));
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
	double pixelsAspectRatio = (double)pXmax / pYmax;
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

__device__ int colorize(_Complex c, unsigned char* row, int iX, int iMax) {
    /** global **/
    unsigned char b; // color
    int i; // iteration
    /** normal map **/
     _Complex Z = 0.0; // initial value for iteration Z0
     _Complex dC = 0.0; // derivative with respect to c
    double reflection = FP_ZERO; // inside
    double h2 = 1.5; // height factor of the incoming light
    double angle = 45.0 / 360.0; // incoming direction of light in turns (change 1st #)
    _Complex v = 0; //cexp(2.0 * angle * M_PI * I); // unit 2D vector in this direction
    v.re = cos(2.0 * angle * M_PI); // this works because the exponent is purely imaginary
    v.im = sin(2.0 * angle * M_PI);
    _Complex u = 0.0; // normal
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
        if (i > i_skip) A += getT(Z);
        R = c_abs(Z);
        /* shape checking algorithm
           skips iterating points within the main cardioid and secondary bulb, otherwise these
           would all hit the max iterations
           removes about 91% of the set from iteration
           REMOVE THIS IF NOT RENDERING THE ENTIRE SET - more computation per iteration if the main
           cardioid and secondary bulb are not shown onscreen */
        double q = ((c.re - 0.25) * (c.im - 0.25)) + (c.im * c.im);
        double cardioid = 0.25 * c.im * c.im;
        double bulb = 0.0625;
        if ((c.re * c.re + 2 * c.re + 1 + c.im * c.im) < bulb || (q * (q +
            (c.re - 0.25)) < cardioid)) {
            break;
        }
        /** get normal map **/
        if (R > escapeRadius) { // exterior of M set
            u = Z / dC;
            u = u / c_abs(u);
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
        de = 2 * R * log(R) / c_abs(dC);
        int thin = 3; // thinness of the border
        if (de < (pixelWidth / thin)) A = FP_ZERO; //  boundary
        else {
            // computing interpolated average
            A /= (i - i_skip); // A(n)
            prevA /= (i - i_skip - 1); // A(n-1)
            // smooth iteration count
            d = i + 1 + log(lnER / log(R)) / M_LN2;
            d = d - (int)d; // only fractional part = interpolation coefficient
            // linear interpolation
            A = d * A + (1.0 - d) * prevA;
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
        row[subPixel + 1] = 0;
        row[subPixel + 2] = 0;
    }

    // exterior of Mandelbrot set -> normal
    else { // multiply the underlying stripe gradient by the reflectivity map
        if (A == FP_ZERO) b = 255; // boundary
        else b = (unsigned char)((254 - (100 * A)) * reflection); // set color bounds for striping

        row[subPixel] = b;
        row[subPixel + 1] = b;
        row[subPixel + 2] = b;
    }
    return 0;
}

int main() {

    _Complex c = 0.0;
    unsigned char row[pXmax * 3];

    setup();

    printf("Rendering row by row");

    for (pY = 0; pY < pYmax; pY++) {
        for (pX = 0; pX < pXmax; pX++) {

        }
            
        
    }



    printf("Render took %.9 lf seconds.\n", gpuTime);
}



void CPUFunction()
{
	printf("hello from the Cpu.\n");
}

__global__
void GPUFunction()
{
	printf("hello from the Gpu.\n");
}

int main()
{
	CPUFunction();

	GPUFunction<<<1, 1 >>>();

	cudaDeviceSynchronize();

	return EXIT_SUCCESS;
}