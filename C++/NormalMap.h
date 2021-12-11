#ifndef C___NORMALMAP_H
#define C___NORMALMAP_H

#include <string>

class NormalMap
{
public:

    std::string get_type();

protected:

    std::string type;

    explicit NormalMap(std::string type);
};


#endif //C___NORMALMAP_H
