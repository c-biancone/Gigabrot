#ifndef C___LINECOLOR_H
#define C___LINECOLOR_H

#include "Shading.h"

class LineColor : public Shading
{
public:

    LineColor();

    ~LineColor();

    unsigned char calculate_bw();

    unsigned char calculate_r();

    unsigned char calculate_g();

    unsigned char calculate_b();
};

#endif //C___LINECOLOR_H