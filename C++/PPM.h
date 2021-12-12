#ifndef C___PPM_H
#define C___PPM_H

#include <fstream>
#include <cstring>
#include <sstream>
#include <array>
#include <vector>
#include <iostream>

class PPM
{
 public:

  /**
   * Default parametrized constructor
   * contains output directory and file name
   * @param width
   * @param height
   */
  PPM(int width, int height);

  /**
   * Minimal parametrized constructor
   * Only needs file name, contains output directory
   * @param fileName
   * @param width
   * @param height
   */
  PPM(const std::string& fileName, int width, int height);

  /**
   * Appends file name to "output" directory destination
   * @param fileNameIn
   */
  void set_filename(const std::string& fileNameIn);

  /**
   * Initialize ofstream
   * @return
   */
  bool init_stream();

  /**
   * Print row of pixels to PPM file - templated for size of array
   * @tparam N - std::array size
   * @param row - std::array of pixels
   */
  //template<size_t N>
  void write_row(const std::vector<unsigned char> &row)
  {
    image.write((char const *) row.data(), row.size());
  }

  /**
   * Print header data to PPM file
   */
  void write_header();

  /**
   * Set width of PPM image
   * @param widthIn
   */
  void set_width(int widthIn);

  /**
   * Set height of PPM image
   * @param heightIn
   */
  void set_height(int heightIn);

  /**
   * Set custom comment for PPM image
   * @param commentIn
   */
  void set_comment(std::string commentIn);

  /**
   * Close ofstream for image
   */
  void close();

 private:

  const std::string magic = "P6\n";

  const std::string pixMaxVal = "255\n";

  int width;

  int subPixel;

  int height;

  std::string comment;

  std::stringstream header;

  std::string outputDirectory;

  std::string fileName;

  std::ofstream image;
};

#endif //C___PPM_H