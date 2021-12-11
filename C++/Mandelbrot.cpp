#include "Mandelbrot.h"

Mandelbrot::Mandelbrot(int pX, int pY)
{
  iterMax = 1000;
  escapeRadius = 1000000.0;
  // lnER = log(escapeRadius);
  c = 0.0;
  r = 0.0;
  z = 0.0;
  dC = 0.0;
  cxMin = -2.2;
  cxMax = 0.8;
  cyMin = -1.5;
  cyMax = 1.5;
  pixWidth = (cxMax-cxMin) / pX;
  pixHeight = (cyMax-cyMin) / pY;
  iSkip = 1;
  thin = 3;
}

Mandelbrot::Mandelbrot(int iterMax, double xMin, double xMax, double yMin, double yMax) : iterMax
(iterMax)
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

void Mandelbrot::interpolate()
{
  // smooth iteration count
  d = iter + 1 + log(log(escapeRadius) / log(r)) / M_LN2;
  d = d - (int) d; // only fractional part = interpolation coefficient
}

void Mandelbrot::average()
{
  a /= (iter - iSkip); // A(n)
  prevA /= (iter - iSkip - 1); // A(n-1)
  this->interpolate();
  a = (d * a) + ((1.0 - d) * prevA);
}

void Mandelbrot::describe_border()
{
  de = 2.0 * r * log(r) / abs(dC);
}

bool Mandelbrot::in_border()
{
  if (de < (pixWidth / thin))
  {
    return true;
  } else {
    return false;
  }
}

bool Mandelbrot::in_set()
{
  if (iter == iterMax)
  {
    return true;
  } else {
    return false;
  }
}
