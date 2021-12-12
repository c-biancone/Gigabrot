#ifndef C____NEUMORPHIC_H_
#define C____NEUMORPHIC_H_

#include "NormalMap.h"

class Neumorphic : public NormalMap
{
 public:

  Neumorphic(std::complex<double> z, std::complex<double> dC);

  double calculate();

  double get_reflection();

  double get_heightFactor();

  double get_angle();

 private:

  std::complex<double> z;

  std::complex<double> dC;

  /**
   * of pseudo incoming light vector
   */
  double heightFactor;

  /**
   * incoming direction of light WRT +x-axis (degrees)
   * change first number
   */
  double angle;

  /**
   * normalized normal vector
   */
  double reflection;

  std::complex<double> u;

  /**
   * unit vector in direction of this.angle
   */
  std::complex<double> v;

};

#endif //C____NEUMORPHIC_H_