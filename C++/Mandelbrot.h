#ifndef C___MANDELBROT_H
#define C___MANDELBROT_H

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

  Mandelbrot(int pX, int pY, int iterMax, double xMin, double xMax, double yMin, double yMax);

  int iterate(size_t iter);

  void set_max_iter(size_t iterMax);

  /**
   * Shape checking algorithm - determines if point is within main cardioid or secondary bulb
   * Removes about 91% of the set from being iterated
   * Should not be implemented for a render that does not include these parts, will add unnecessary
   * computing
   * @param c - complex number location
   * @return TRUE if within the main shapes
   */
  bool shape_check(std::complex<double> c);

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
  double pX;

  double pY;

  double pixWidth;

  double pixHeight;

  // average
  double a;

  double prevA;

  /**
   * Exclude iSkip+1 elements from average
   */
  int iSkip;

  // interpolated
  double d;

  // boundary descriptor
  double de;
  int thin;
};

#endif //C___MANDELBROT_H