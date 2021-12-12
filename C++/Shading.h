#ifndef C___SHADING_H
#define C___SHADING_H

#include "Colorization.h"

class Shading : public Colorization
{
 public:

  explicit Shading(std::string subtype);

  ~Shading();

  virtual unsigned char get_max_color_value();

  virtual unsigned char get_min_color_value();

  virtual unsigned char calculate_bw() = 0;

  // option for other colorizations
  virtual unsigned char calculate_r() = 0;

  virtual unsigned char calculate_g() = 0;

  virtual unsigned char calculate_b() = 0;

 private:

  std::string type = "Shading";
};

#endif  // C___SHADING_H
