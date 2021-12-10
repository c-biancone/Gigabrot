#include "Mandelbrot.h"

Mandelbrot::Mandelbrot(int iterMax, double xMin, double xMax, double yMin, double yMax) : iterMax
(iterMax),
{}

bool Mandelbrot::shape_check(std::complex<double> c)
{
    q = ((real(c) - 0.25) * (real(c) - 0.25)) + (imag(c) * imag(c));
    cardioid = 0.25 * imag(c) * imag(c);
    if ((real(c) * real(c) + 2 * real(c) + 1 + imag(c) * imag(c)) < bulb ||
        (q * (q +(real(c) - 0.25)) < cardioid))
    {
        return true;
    } else {
        return false;
    }
}
