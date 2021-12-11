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

  virtual unsigned char get_max_color_value() = 0;

  virtual ~Colorization();

 protected:
  /**
   * Determines type of colorization
   */
  std::string type;

  std::string subtype;

  const unsigned char maxColorValue = 255;

  const unsigned char minColorValue = 0;

  unsigned char r;

  unsigned char g;

  unsigned char b;

  /**
   * Parametrized constructor
   * @param type of colorization
   */
  explicit Colorization(std::string type);
};

#endif  // C___COLORIZATION_H
