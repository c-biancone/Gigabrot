#ifndef C___MANDELBROT_H
#define C___MANDELBROT_H

#include "Shading.h"
#include "InsideColor.h"
#include "LineColor.h"
#include "Striping.h"
#include <cmath>
#include <complex>
#include <algorithm>
#include <iostream>

#define M_PI 3.14159265358979323846

class Mandelbrot
{
 public:

  /**
   * default parametrized constructor
   * @param width - image width
   * @param height - image height
   */
  Mandelbrot(int width, int height);

  /**
   * minimal parametrized constructor
   * used for calling within pixel loops
   * @param pX - current pixel x
   * @param pY - current pixel y
   * @param width - image width
   * @param height - image height
   */
  Mandelbrot(int pX, int pY, int width, int height);

  // a few too many parameters to choose right now
  //Mandelbrot(int pX, int pY, int iterMax, double xMin, double xMax, double yMin, double yMax);

  ~Mandelbrot();

  void current_pixel(int pxIn, int pyIn);

  void set_image(int widthIn, int heightIn);

  void set_plane(double cxMinIn, double cxMaxIn, double cyMinIn, double cYMaxIn);

  void set_stripe_density(double stripeDensityIn);

  void set_iSkip(int iSkipIn);

  void set_border(int thinIn);

  void get_c();

  void iterate();

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

  void check_distortion();

  void reset();


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
  int width;

  int height;

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