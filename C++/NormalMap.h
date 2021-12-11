#ifndef C___NORMALMAP_H
#define C___NORMALMAP_H

#include <string>

class NormalMap
{
public:

    std::string get_type();


protected:

    explicit NormalMap(std::string type);

    std::string type;

    const double minMapVal = 0.0;

    const double maxMapVal = 1.0;
};


#endif //C___NORMALMAP_H
