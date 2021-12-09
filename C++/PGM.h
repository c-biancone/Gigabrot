#ifndef C___PGM_H
#define C___PGM_H

#include <fstream>
#include <cstring>
#include <sstream>

class PGM
{
public:

    PGM(std::string fileName, int width, int height);

    bool init_stream();

    bool write_row(std::string fileName, unsigned char * row);

    void write_header();

    void set_width(int widthIn);

    void set_height(int heightIn);

    void set_comment(std::string commentIn);

    void set_magic();

    //static void set_comment();

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