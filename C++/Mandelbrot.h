#ifndef C___MANDELBROT_H
#define C___MANDELBROT_H

#include "Shading.h"
#include "InsideColor.h"
#include "LineColor.h"
#include "Striping.h"
#include <cmath>
#include <complex>
#include <algorithm>

#define M_PI 3.14159265358979323846

class Mandelbrot
{
 public:

  /**
   * default constructor
   * @param pX - image width
   * @param pY - image height
   */
  Mandelbrot(int pX, int pY);

  //Mandelbrot(int pX, int pY, int iterMax, double xMin, double xMax, double yMin, double yMax);

  ~Mandelbrot();

  void get_c();

  int iterate();

  unsigned char colorize_bw();

  /**
   * Shape checking algorithm - determines if point is within main cardioid or secondary bulb
   * Removes about 91% of the set from being iterated
   * Should not be implemented for a render that does not include these parts, will add unnecessary
   * computing
   * @return TRUE if within the main shapes
   */
  bool shape_check();

  double get_t();

  void interpolate();

  void average();

  void describe_border();

  bool in_border();

  bool in_set(); // for readability


 private:

  int iter;

  int iterMax;

  double escapeRadius;

  // double lnER;

  std::complex<double> c;

  double r;

  std::complex<double> z;

  std::complex<double> dC;

  double q;

  double cardioid;

  const double bulb = 0.0625;

  // coordinate plane
  double cxMin;

  double cxMax;

  double cyMin;

  double cyMax;

  // image
  int pX;

  int pY;

  double pixWidth;

  double pixHeight;

  // average
  double a;

  double prevA;

  /**
   * Higher is more dense
   */
  double stripeDensity;

  /**
   * Exclude iSkip+1 elements from average
   */
  int iSkip;

  // interpolated
  double d;

  // boundary descriptor
  double de;
  int thin;

  Shading *shade;

};

#endif //C___MANDELBROT_H