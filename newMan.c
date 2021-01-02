
#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <time.h>
#include <omp.h>

#define M_PI 3.14159265358979323846 /* pi */

/* screen ( integer) coordinate */
int iX, iY;
const int iXmax = 500;
const int iYmax = 501; // for main antenna

const int iterationMax = 256; //  N in wiki


/* world ( double) coordinate = parameter plane*/
double cx, cy;
const double cxMin = -2.2;
const double cxMax = 0.8;
const double cyMin = 1.5;
const double cyMax = -1.5;
/* */
double pixelWidth; //=(CxMax-CxMin)/iXmax;
double pixelHeight; // =(CyMax-CyMin)/iYmax;
/* color component ( R or G or B) is coded from 0 to 255; it is 24 bit color RGB file */
const int maxColorComponentValue = 255;
FILE * fp;
char * filename = "mandelbrot.ppm"; //
char * comment = "# "; /* comment should start with # */

static unsigned char color[3]; // 24-bit rgb color
unsigned char s = 7; // stripe density

int iSkip = 2; // exclude (i_skip+1) elements from average
/* bail-out value for the bailout test for escaping points
 radius of circle centered ad the origin, exterior of such circle is a target set  */
const double escapeRadius = 1000000; // big !!!!
double lnER; // ln(ER)



void info() {

    double distortion;
    // width/height
    double pixelsAspectRatio = (double) iXmax / iYmax;
    double worldAspectRatio = (cxMax - cxMin) / (cyMax - cyMin);
    printf("PixelsAspectRatio = %.16f \n", pixelsAspectRatio);
    printf("WorldAspectRatio = %.16f \n", worldAspectRatio);
    distortion = pixelsAspectRatio - worldAspectRatio;
    printf("distortion = %.16f ( it should be zero !)\n", distortion);
    printf("bailout value = Escape Radius = %.0f \n", escapeRadius);
    printf("IterationMax = %d \n", iterationMax);
    printf("i_skip = %d = number of skipped elements ( including t0 )= %d \n", iSkip, iSkip+1);

    // file
    printf("file %s saved.\n", filename);

}

void close() {
    fclose(fp);
    info();
}

int main()
{
    double execTime = 0.0;
    clock_t begin = clock();
    double _Complex C;

// setup
    pixelWidth = (cxMax - cxMin) / iXmax;
    pixelHeight = (cyMax - cyMin) / iYmax;
    //open file
    fp = fopen(filename, "wb");
    // write ASCII header
    fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, iXmax, iYmax, maxColorComponentValue);

// iteration
    for (iY = 0; iY < iYmax; iY++)
    {
        for (iX = 0; iX < iXmax; iX++)
        {
            // compute pixel coordinate
            cy = cyMax - iY * pixelHeight;
            cx = cxMin + iX * pixelWidth;
            C = cx + cy * I;

            // insert shape checking here?

            //compute color
            double reflection = 1.0;
            unsigned char b;

            //vars for both vector map and stripe average
            double _Complex Z = 0.0;
            double _Complex dC = 0.0;
            // reflection vars
            double h = 1.5; // incoming light vector height factor
            double angle = 45.0; // incoming light source angle if picture viewed as unit
            // circle
            double _Complex v = cexp(2.0 * M_PI * (360 - (angle/360)) * I); // light vector
            double _Complex u; // surface vector
            // arg vars
            double A = 0.0; // A(n)
            double prevA = 0.0; // A(n-1)
            double R; // radius = cabs(Z)
            double d; // smooth iteration count
            double de; // determine boundary for striping
            int i = 0;


            for (i = 0; i < iterationMax; i++)
            {
                dC = 2 * Z * dC + 1;
                Z = Z * Z + C; // the actual sauce

                // get reflection
                if (cabs(Z) > escapeRadius) {
                    u = Z / dC;
                    u = u / cabs(u);
                    reflection = creal(u) * creal(v) + cimag(u) * cimag(v); // dot product
                    reflection = reflection / (1.0 + h);
                    if (reflection < 0.0) reflection = 0.0;
                    break;
                }
                // get arg
/*
                if (i > iSkip) A += (0.5 + 0.5 * sin(s * carg(Z)));
                R = cabs(Z);
                if (cabs(Z) > escapeRadius) break; // exterior of set
                prevA = A; // save value for interpolation
*/
            }

            if (i == iterationMax)
                A = -1.0; // interior
            else
            {
                de = 2 * R * log(R) / cabs(dC);
                if (de < pixelWidth) A = FP_ZERO; //boundary
                else
                {
                    // compute interpolated average
                    A /= (i - iSkip); // A(n)
                    prevA /= (i - iSkip - 1); // A(n-1)
                    // smooth iteration count
                    d = i + 1 + log(lnER / log(R)) / M_LN2;
                    d = d - (int)d; // only fractional part = interpolation coefficient
                    // linear interpolation
                    A = d * A + (1.0 - d) * prevA;
                }
            }


            if (reflection == FP_ZERO) // inside
            {
                color[0] = 0;
                color[1] = 0;
                color[2] = 0;
            } else
            {
                if (A == FP_ZERO) b = 255; // border
                else b = (unsigned char) ((254 - (100)) * reflection); // outside

                color[0] = b;
                color[1] = b;
                color[2] = b;
            }

            fwrite(color, 1, 3, fp);
        }
    }
    close();
    clock_t end = clock();
    execTime += (double) (end - begin) / CLOCKS_PER_SEC;
    printf("Time elapsed: %f seconds\n", execTime);

    return 0;
}