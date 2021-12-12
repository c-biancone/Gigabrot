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

  virtual unsigned char get_max_color_value() = 0;

  virtual unsigned char get_min_color_value() = 0;

  virtual ~Colorization();

 protected:

  /**
   * Parametrized constructor
   * @param type of colorization
   */
  explicit Colorization(std::string type);

  /**
   * Determines type of colorization
   */
  std::string type;

  const unsigned char maxColorValue = 255;

  const unsigned char minColorValue = 0;

};

#endif  // C___COLORIZATION_H
