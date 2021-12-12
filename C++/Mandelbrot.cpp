#include "Mandelbrot.h"

using namespace std;

Mandelbrot::Mandelbrot(int width, int height) : width(width), height(height)
{
  pX = 0;
  pY = 0;
  iter = 0;
  iterMax = 1000;
  escapeRadius = 1000000.0;
  // lnER = log(escapeRadius);
  c = 0.0;
  r = 0.0;
  z = 0.0;
  dC = 0.0;
  q = 0.0;
  cardioid = 0.0;
  a = 0.0;
  prevA = 0.0;
  stripeDensity = 7.0;
  d = 0.0;
  de = 0.0;
  cxMin = -2.2;
  cxMax = 0.8;
  cyMin = -1.5;
  cyMax = 1.5;
  pixWidth = 0.0;
  pixHeight = 0.0;
  iSkip = 1;
  thin = 3;
  shade = nullptr; // avoid calling "new" more than once per pixel
}

Mandelbrot::Mandelbrot(int pX, int pY, int width, int height) : pX(pX), pY(pY), width(width),
height(height)
{
  iter = 0;
  iterMax = 1000;
  escapeRadius = 1000000.0;
  // lnER = log(escapeRadius);
  c = 0.0;
  r = 0.0;
  z = 0.0;
  dC = 0.0;
  q = 0.0;
  cardioid = 0.0;
  a = 0.0;
  prevA = 0.0;
  stripeDensity = 7.0;
  d = 0.0;
  de = 0.0;
  cxMin = -2.2;
  cxMax = 0.8;
  cyMin = -1.5;
  cyMax = 1.5;
  pixWidth = 0.0;
  pixHeight = 0.0;
  iSkip = 1;
  thin = 3;
  shade = nullptr; // avoid calling "new" more than once per pixel
}

Mandelbrot::~Mandelbrot()
{
  delete shade;
}

void Mandelbrot::set_image(int widthIn, int heightIn)
{
  width = widthIn;
  height = heightIn;
}

void Mandelbrot::current_pixel(int pxIn, int pyIn)
{
  pX = pxIn;
  pY = pyIn;
}

void Mandelbrot::set_plane(double cxMinIn, double cxMaxIn, double cyMinIn, double cYMaxIn)
{
  cxMin = cxMinIn;
  cxMax = cxMaxIn;
  cyMin = cyMinIn;
  cyMax = cYMaxIn;
}

void Mandelbrot::set_stripe_density(double stripeDensityIn)
{
  stripeDensity = stripeDensityIn;
}

void Mandelbrot::set_iSkip(int iSkipIn)
{
  iSkip = iSkipIn;
}

void Mandelbrot::set_border(int thinIn)
{
  thin = thinIn;
}

void Mandelbrot::get_c()
{
  pixWidth = (cxMax-cxMin) / width;
  pixHeight = (cyMax-cyMin) / height;
  c = (cxMin + pX * pixWidth) + ((cyMax - pY * pixHeight) * 1i);
}

void Mandelbrot::iterate()
{
  if (!this->shape_check())
  {
    for (iter = 0; iter < iterMax; iter++)
    {
      // mandelbrot set formula
      dC = 2.0 * dC * z + 1.0;
      z = z * z + c;

      // compute average
      if (iter > iSkip)
      {
        a += get_t();
      }

      r = abs(z);
      if (r > escapeRadius)
      {
        break;
      }

      prevA = a;
    }

    average();
  }
}

unsigned char Mandelbrot::colorize_bw()
{
  if (in_set())
  {
    shade = new InsideColor();
    InsideColor *color;
    color = dynamic_cast<InsideColor*>(shade);
    return color->calculate_bw();
  } else if (a == FP_ZERO) {
    shade = new LineColor();
    LineColor *color;
    color = dynamic_cast<LineColor*>(shade);
    return color->calculate_bw();
  } else {
    shade = new Striping(a, z, dC);
    Striping *color;
    color = dynamic_cast<Striping*>(shade);
    return color->calculate_bw();
  }
}

bool Mandelbrot::shape_check()
{
    q = ((real(c) - 0.25) * (real(c) - 0.25)) + (imag(c) * imag(c));
    cardioid = 0.25 * imag(c) * imag(c);
    if ((real(c) * real(c) + 2.0 * real(c) + 1.0 + imag(c) * imag(c)) < bulb ||
        (q * (q +(real(c) - 0.25)) < cardioid))
    {
        return true;
    } else {
        return false;
    }
}

double Mandelbrot::get_t()
{
  return 0.5 + 0.5 * sin(stripeDensity * arg(z));
}

void Mandelbrot::interpolate()
{
  // smooth iteration count
  d = iter + 1 + log(log(escapeRadius) / log(r)) / M_LN2;
  d = d - (int) d; // only fractional part = interpolation coefficient
}

void Mandelbrot::average()
{
  if (in_set())
  {
    a = -1.0;
  } else {
    describe_border();
    if (in_border()) // in border
    {
      a = FP_ZERO;
    } else {
      a /= (iter - iSkip); // A(n)
      prevA /= (iter - iSkip - 1); // A(n-1)
      this->interpolate();
      a = (d * a) + ((1.0 - d) * prevA);
    }
  }
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

std::ostream &operator<<(ostream &os, const Mandelbrot& mandelbrot)
{
  double pixAspectRatio = (static_cast<double>(mandelbrot.width) / static_cast<double>(mandelbrot
      .height));
  double worldAspectRatio = (mandelbrot.cxMax - mandelbrot.cxMin) / (mandelbrot.cyMax -
      mandelbrot.cyMin);
  double distortion = pixAspectRatio - worldAspectRatio;
  os << "Distortion (should be 0): " << distortion << "\n";
}

void Mandelbrot::reset()
{
  c = 0.0;
  r = 0.0;
  z = 0.0;
  dC = 0.0;
  q = 0.0;
  cardioid = 0.0;
  a = 0.0;
  prevA = 0.0;
  d = 0.0;
  shade = nullptr; // avoid calling "new" more than once per pixel
}


