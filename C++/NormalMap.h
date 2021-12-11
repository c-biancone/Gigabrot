#ifndef C___NORMALMAP_H
#define C___NORMALMAP_H

#include <string>
#include <complex>
#include <cmath>

class NormalMap
{
 public:

  std::string get_type();

  virtual double calculate() = 0;

  double dot_product(std::complex<double> u, std::complex<double> v);

 protected:

  explicit NormalMap(std::string type);

  const double minMapVal = 0.0;

  const double maxMapVal = 1.0;

  std::string type;

 private:

};

#endif //C___NORMALMAP_H
