#include "Colorization.h"

#include <utility>

using namespace std;

Colorization::Colorization(string type) : type(std::move(type))
{}

std::string Colorization::get_type()
{
  return type;
}

Colorization::~Colorization() = default;