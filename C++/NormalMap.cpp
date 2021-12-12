#include "NormalMap.h"

using namespace std;

NormalMap::NormalMap(string type)
{}

double NormalMap::dot_product(std::complex<double> u, std::complex<double> v)
{
  return real(u) * real(v) + imag(u) * imag(v);
}

double NormalMap::get_min_val()
{
  return minMapVal;
}

double NormalMap::get_max_val()
{
  return maxMapVal;
}
std::string NormalMap::get_type()
{
  return type;
}