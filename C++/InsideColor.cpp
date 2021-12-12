#include "InsideColor.h"

InsideColor::InsideColor() : Shading("Inside")
{}

InsideColor::~InsideColor()
= default;

unsigned char InsideColor::calculate_bw()
{
  return minColorValue;
}

unsigned char InsideColor::calculate_r()
{
  return minColorValue;
}

unsigned char InsideColor::calculate_g()
{
  return minColorValue;
}

unsigned char InsideColor::calculate_b()
{
  return minColorValue;
}