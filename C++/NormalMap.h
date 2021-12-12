#ifndef C___NORMALMAP_H
#define C___NORMALMAP_H

#include <string>
#include <complex>
#include <cmath>

class NormalMap
{
 public:

  ~NormalMap();

  virtual double calculate() = 0;

  /**
   * Complex number dot product
   * @param u
   * @param v
   * @return
   */
  double dot_product(std::complex<double> u, std::complex<double> v);

  double get_min_val();

  double get_max_val();

  std::string get_type();

 protected:

  explicit NormalMap(std::string type);

 private:

  const double minMapVal = 0.0;

  const double maxMapVal = 1.0;

  std::string type;
};

#endif //C___NORMALMAP_H