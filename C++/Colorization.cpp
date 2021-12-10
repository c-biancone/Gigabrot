#include "Colorization.h"

#include <utility>

using namespace std;

Colorization::Colorization(string type) : type(std::move(type))
{}

std::string Colorization::get_type()
{
    return type;
}

void Colorization::set_type(string typeIn)
{
    type = std::move(typeIn);
}

Colorization::~Colorization() = default;