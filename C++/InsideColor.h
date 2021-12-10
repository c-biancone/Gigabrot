#ifndef C___INSIDECOLOR_H
#define C___INSIDECOLOR_H

#include "Shading.h"

class InsideColor : public Shading
{
public:

    InsideColor();

    int calculate_r();

    int calculate_g();

    int calculate_b();
};


#endif //C___INSIDECOLOR_H