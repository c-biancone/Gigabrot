#ifndef C___STRIPING_H
#define C___STRIPING_H

#include "Shading.h"

class Striping : public Shading
{
public:

    Striping(double average);

    unsigned char calculate_bw();

    unsigned char calculate_r();

    unsigned char calculate_g();

    unsigned char calculate_b();

    double get_average(double average);

private:

    double average;
};


#endif //C___STRIPING_H