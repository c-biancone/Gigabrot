#ifndef C___MANDELBROT_H
#define C___MANDELBROT_H

#include <cmath>
#include <complex>
#include <algorithm>

#define M_PI 3.14159265358979323846

class Mandelbrot
{
public:

    Mandelbrot(int iterMax, double xMin, double xMax, double yMin, double yMax);

    int iterate(size_t iter);

    void set_max_iter(size_t iterMax);

    /**
     * Shape checking algorithm - determines if point is within main cardioid or secondary bulb
     * Removes about 91% of the set from being iterated
     * Should not be implemented for a render that does not include these parts, will add unnecessary
     * computing
     * @param c - complex number location
     * @return TRUE if within the main shapes
     */
    bool shape_check(std::complex<double> c);

    int iter;

private:

    const int iterMax = 1000;

    std::complex<double> c;

    std::complex<double> r;

    std::complex<double> z;

    std::complex<double> dC;

    double q;

    double cardioid;

    const double bulb = 0.0625;

    // coordinate plane
    double cxMin;

    double cxMax;

    double cyMin;

    double cyMax;

};

#endif //C___MANDELBROT_H