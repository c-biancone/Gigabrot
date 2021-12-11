#ifndef C___NORMALMAP_H
#define C___NORMALMAP_H

#include <string>
#include <complex>

class NormalMap
{
 public:

  std::string get_type();

  virtual double calculate() = 0;

 protected:

  explicit NormalMap(std::string type);

  const double minMapVal = 0.0;

  const double maxMapVal = 1.0;

  std::complex<double> z;

  std::complex<double> dC;

 private:
  std::string type;
};

#endif //C___NORMALMAP_H
