#ifndef C___STRIPING_H
#define C___STRIPING_H

#include "Shading.h"
#include "Neumorphic.h"

class Striping : public Shading
{
 public:

  Striping(double average, std::complex<double> z, std::complex<double> dc);

  ~Striping();

  unsigned char calculate_bw();

  unsigned char calculate_r();

  unsigned char calculate_g();

  unsigned char calculate_b();

  double get_average(double average);

 private:

  double average;

  Neumorphic reflection;
};

#endif //C___STRIPING_H