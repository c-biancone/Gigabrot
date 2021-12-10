//
// Created by Chris on 12/10/2021.
//

#ifndef C___COLORIZATION_H
#define C___COLORIZATION_H

#include <string>

class Colorization
{
    /**
     * Parametrized constructor
     * @param type of colorization
     */
    explicit Colorization(std::string type);

    /**
     * @return type of colorization
     */
    std::string get_type();

    /**
     * @param typeIn type of colorization
     */
    void set_type(std::string typeIn);

    /**
     * Implemented at
     * @param i
     * @param iMax
     * @return
     */
    virtual int calculate(int i, int iMax) = 0;

    virtual ~Colorization();

protected:

    /**
     * Determines type of colorization
     */
    std::string type;

    /**
     * 0 - 255
     */
    int pixelValue;
};


#endif //C___COLORIZATION_H
