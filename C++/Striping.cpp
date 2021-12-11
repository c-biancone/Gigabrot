#include "Striping.h"

using namespace std;

Striping::Striping(double average, complex<double> z, complex<double> dc) : Shading("Striping"),
average(average), reflection(z, dc)
{}

unsigned char Striping::calculate_bw()
{
  return static_cast<unsigned char>((static_cast<double>(maxColorValue) - 1.0) - (100.0 *
  average) * reflection.calculate());
}

unsigned char Striping::calculate_r()
{
  return 0;
}

unsigned char Striping::calculate_g()
{
  return 0;
}

unsigned char Striping::calculate_b()
{
  return 0;
}

double Striping::get_average(double a)
{
  return average;
}