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

    /**
     * @return value of pixel color
     */
    virtual int calculate() = 0;

    int get_max_color_value();

private:

    std::string type = "Shading";


};


#endif //C___SHADING_H
