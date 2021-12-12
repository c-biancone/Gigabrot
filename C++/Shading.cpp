#include "Shading.h"

#include <utility>

Shading::Shading(std::string type) : Colorization(std::move(type))
{}

Shading::~Shading()
= default;

unsigned char Shading::get_max_color_value()
{
  return maxColorValue;
}

unsigned char Shading::get_min_color_value()
{
  return minColorValue;
}