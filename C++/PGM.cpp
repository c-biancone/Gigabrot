#include "PGM.h"

#include <utility>

using namespace std;

PGM::PGM(std::string fileName, int width, int height) : fileName(std::move(fileName)), width
    (width), height(height)
{}

bool PGM::init_stream()
{
    image.open(fileName);

    if (image.is_open())
    {
        return true;
    } else {
        return false;
    }
}

void PGM::write_header()
{
    string widthStr = to_string(this->width);
    string lengthStr = to_string(this->height);
    header << magic << widthStr << " " << lengthStr << "\n" << comment << "\n" << pixMaxVal;

    image << header.rdbuf();
}

bool PGM::write_row(std::string fileName, unsigned char *row)
{
    return false;
}

void PGM::set_width(int widthIn)
{
    this->width = widthIn;
}

void PGM::set_height(int heightIn)
{
    this->height = heightIn;
}

void PGM::set_comment(string commentIn)
{
    this->comment = std::move(commentIn);
}

void PGM::close()
{
    image.close();
}