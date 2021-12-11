#ifndef C____NEUMORPHIC_H_
#define C____NEUMORPHIC_H_

#include "NormalMap.h"

class Neumorphic : public NormalMap
{
 public:

  Neumorphic(std::complex<double> z, std::complex<double> dC);

  double calculate();

  double get_reflection();

 private:

  std::complex<double> z;

  std::complex<double> dC;

  /**
   * height factor of incoming light
   */
  double h2 = 1.5;

  /**
   * incoming direction of light WRT +x-axis (degrees)
   * change first number
   */
  double angle = 45.0 / 360.0;

  /**
   * normalized normal vector
   */
  double reflection;

};

#endif //C____NEUMORPHIC_H_
