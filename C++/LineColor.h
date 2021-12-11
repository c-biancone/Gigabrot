#ifndef C___LINECOLOR_H
#define C___LINECOLOR_H

#include "Shading.h"

class LineColor : public Shading
{
public:

    LineColor();

    int calculate_r();

    int calculate_g();

    int calculate_b();

};

#endif //C___LINECOLOR_H