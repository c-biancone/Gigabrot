#include "Striping.h"

using namespace std;

Striping::Striping(double average, complex<double> z, complex<double> dc) : Shading("Striping"),
average(average), reflection(z, dc)
{}

Striping::~Striping()
= default;

unsigned char Striping::calculate_bw()
{
  return static_cast<unsigned char>(((maxColorValue - 1) - (100.0 *
  average)) * reflection.calculate()); // explicit casting
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