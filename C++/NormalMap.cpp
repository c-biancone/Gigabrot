#include "NormalMap.h"

using namespace std;

NormalMap::NormalMap(string type)
{}

std::string NormalMap::get_type()
{
    return type;
}

double NormalMap::dot_product(std::complex<double> u, std::complex<double> v)
{
  return real(u) * real(v) + imag(u) * imag(v);
}
