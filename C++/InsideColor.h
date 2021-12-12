#ifndef C___INSIDECOLOR_H
#define C___INSIDECOLOR_H

#include "Shading.h"

class InsideColor : public Shading
{
 public:

  InsideColor();

  unsigned char calculate_bw();

  unsigned char calculate_r();

  unsigned char calculate_g();

  unsigned char calculate_b();
};

#endif //C___INSIDECOLOR_H