#include "Shading.h"

#include <utility>

Shading::Shading(std::string type) : Colorization(std::move(type))
{}

void Shading::set_subtype(std::string subtypeIn)
{
    subtype = subtypeIn;
}

unsigned char Shading::get_max_color_value()
{
    return maxColorValue;
}

Shading::~Shading()
= default;