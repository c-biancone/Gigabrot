#include "Neumorphic.h"

using namespace std;

Neumorphic::Neumorphic(complex<double> z, complex<double> dC) : NormalMap("Neumorphic"), z(z), dC
(dC)
{}

double Neumorphic::calculate()
{
  complex<double> u = z / dC;
  u = u / abs(u); // normalize
  complex<double> v = exp(2.0 * angle * M_PI * 1i); // unit 2D vector in this direction
  reflection = dot_product(u, v) + h2;
  reflection /= (1.0 + h2); // rescale so that t does not get bigger than 1
  if (reflection < 0.0)
  {
    reflection = 0.0;
  } else {}
  return reflection;
}

double Neumorphic::get_reflection()
{
  return reflection;
}

