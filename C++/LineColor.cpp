#include "LineColor.h"

LineColor::LineColor() : Shading("Line")
{

}

int LineColor::calculate_r()
{
    r = 255;
    return 255;
}

int LineColor::calculate_g()
{
    g = 255;
    return g;
}

int LineColor::calculate_b()
{
    b = 255;
    return g;
}