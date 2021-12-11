#include "Striping.h"

Striping::Striping(double average) : Shading("Striping"), average(average)
{}

unsigned char Striping::calculate_bw()
{
  return (unsigned char) (maxColorValue - 1) - (100 * average);
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