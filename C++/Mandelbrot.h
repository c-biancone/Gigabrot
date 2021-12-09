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

    int iter;

private:

    const int iterMax;

    std::complex<double> r;

    std::complex<double> z;

    std::complex<double> dC;

    double q;

    double cardioid;

    const double bulb;


};

#endif //C___MANDELBROT_H