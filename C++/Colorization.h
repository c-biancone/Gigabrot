//
// Created by Chris on 12/10/2021.
//

#ifndef C___COLORIZATION_H
#define C___COLORIZATION_H

#include <string>

class Colorization
{

public:
    /**
     * @return type of colorization
     */
    std::string get_type();

    /**
     * @param typeIn type of colorization
     */
    void set_type(std::string typeIn);

    /**
     * @param subtypeIn of colorization type
     */
    virtual void set_subtype(std::string subtypeIn) = 0;

    virtual int get_max_color_value() = 0;

    virtual ~Colorization();

protected:

    /**
     * Determines type of colorization
     */
    std::string type;

    std::string subtype;

    const int maxColorValue = 255;

/**
 * Parametrized constructor
 * @param type of colorization
 */
explicit Colorization(std::string subtype);
};


#endif //C___COLORIZATION_H
