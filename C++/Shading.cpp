#include "Shading.h"

#include <utility>

Shading::Shading(std::string type) : Colorization(std::move(type))
{}

unsigned char Shading::get_max_color_value()
{
  return maxColorValue;
}

unsigned char Shading::get_min_color_value()
{
  return minColorValue;
}

Shading::~Shading()
= default;