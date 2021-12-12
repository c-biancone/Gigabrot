#include "Neumorphic.h"

using namespace std;

Neumorphic::Neumorphic(complex<double> z, complex<double> dC) : NormalMap("Neumorphic"), z(z), dC
(dC)
{
  heightFactor = 1.5;
  angle = 45.0 / 360.0;
  reflection = FP_ZERO;
  v = exp(2.0 * angle * M_PI * 1i);
}

double Neumorphic::calculate()
{
  u = z / dC;
  u = u / abs(u); // normalize
  reflection = dot_product(u, v) + heightFactor;
  reflection = reflection / (1.0 + heightFactor); // rescale so that it does not get bigger than 1
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

double Neumorphic::get_heightFactor()
{
  return heightFactor;
}

double Neumorphic::get_angle()
{
  return angle;
}