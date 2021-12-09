//
// Created by Chris on 12/7/2021.
//

#ifndef C___MANDELBROTCONFIGABSTRACT_H
#define C___MANDELBROTCONFIGABSTRACT_H


class MandelbrotConfigAbstract
{
public:
    virtual ~MandelbrotConfigAbstract() = default;

    virtual int get_max_iter() const = 0;

};


#endif //C___MANDELBROTCONFIGABSTRACT_H
