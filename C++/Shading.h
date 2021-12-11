//
// Created by Chris on 12/10/2021.
//

#ifndef C___SHADING_H
#define C___SHADING_H


#include "Colorization.h"

class Shading : public Colorization
{
public:

    explicit Shading(std::string subtype);

    ~Shading();

    void set_subtype(std::string subtypeIn);

    int get_max_color_value();

    virtual int calculate_r() = 0;

    virtual int calculate_g() = 0;

    virtual int calculate_b() = 0;

private:

    std::string type = "Shading";


};


#endif //C___SHADING_H
