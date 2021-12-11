#ifndef C___SHADING_H
#define C___SHADING_H

#include "Colorization.h"

class Shading : public Colorization {
 public:
  explicit Shading(std::string subtype);

  ~Shading();

  void set_subtype(std::string subtypeIn);

  unsigned char get_max_color_value();

  virtual unsigned char calculate_bw() = 0;

  virtual unsigned char calculate_r() = 0;

  virtual unsigned char calculate_g() = 0;

  virtual unsigned char calculate_b() = 0;

 private:
  std::string type = "Shading";
};

#endif  // C___SHADING_H
