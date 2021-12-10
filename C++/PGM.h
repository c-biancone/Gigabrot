#ifndef C___PGM_H
#define C___PGM_H

#include <fstream>
#include <cstring>
#include <sstream>
#include <array>
#include <vector>

class PGM
{
public:

    /**
     * Parametrized constructor
     * @param fileName
     * @param width
     * @param height
     */
    PGM(std::string fileName, int width, int height);

    /**
     * Initialize ofstream
     * @return
     */
    bool init_stream();

    /**
     * Print row of pixels to PGM file - templated for size of array
     * @tparam N - std::array size
     * @param row - std::array of pixels
     */
    //template<size_t N>
    void write_row(const std::vector<unsigned char>& row)
    {
        image.write((char const*)row.data(), row.size());
    }

    /**
     * Print header data to PGM file
     */
    void write_header();

    /**
     * Set width of PGM image
     * @param widthIn
     */
    void set_width(int widthIn);

    /**
     * Set height of PGM image
     * @param heightIn
     */
    void set_height(int heightIn);

    /**
     * Set custom comment for PGM image
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

    int height;

    std::string comment;

    std::stringstream header;

    std::string fileName;

    std::ofstream image;
};

#endif //C___PGM_H