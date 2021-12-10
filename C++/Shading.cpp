//
// Created by Chris on 12/10/2021.
//

#include "Shading.h"

#include <utility>

Shading::Shading(std::string subtype) : Colorization(std::move(subtype))
{}

void Shading::set_subtype(std::string subtypeIn)
{
    subtype = subtypeIn;
}

int Shading::get_max_color_value()
{
    return maxColorValue;
}

Shading::~Shading()
= default;