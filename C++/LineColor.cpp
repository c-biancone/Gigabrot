#include "LineColor.h"

LineColor::LineColor() : Shading("Line")
{}

LineColor::~LineColor()
= default;

unsigned char LineColor::calculate_bw()
{
  return maxColorValue;
}

unsigned char LineColor::calculate_r()
{
  return maxColorValue;
}

unsigned char LineColor::calculate_g()
{
  return maxColorValue;
}

unsigned char LineColor::calculate_b()
{
  return maxColorValue;
}