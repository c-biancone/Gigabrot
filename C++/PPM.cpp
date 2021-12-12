#include "PPM.h"

#include <utility>

using namespace std;

PPM::PPM(int width, int height) : width(width), height(height)
{
  outputDirectory = "..\\..\\output\\";
  fileName = outputDirectory + "gigabrot_default.ppm";
  subPixel = width * 3;
}

PPM::PPM(const std::string &fileName, int width, int height) : width(width), height(height)
{
  outputDirectory = "..\\..\\output\\";
  this->fileName = outputDirectory + fileName;
  subPixel = width * 3;
}

PPM::PPM(const PPM& oldPPM) : magic(oldPPM.magic), pixMaxVal(oldPPM.pixMaxVal),
    width(oldPPM.width), height(oldPPM.height), subPixel(oldPPM.subPixel), comment(oldPPM.comment)
{}

void PPM::set_outputDirectory(const std::string &outputDirectoryIn)
{
  outputDirectory = outputDirectoryIn;
}

PPM& PPM::operator=(string fileNameIn)
{
  this->fileName = std::move(fileNameIn);
  return *this;
}

bool PPM::init_stream()
{
  image.open(fileName, ios::binary);

  if (image.is_open())
  {
    return true;
  } else
  {
    return false;
  }
}

void PPM::write_header()
{
  string widthStr = to_string(this->width);
  string lengthStr = to_string(this->height);
  header << magic << widthStr << " " << lengthStr << "\n" << comment << "\n" << pixMaxVal;
  image << header.rdbuf();
}

void PPM::set_width(int widthIn)
{
  width = widthIn;
}

void PPM::set_height(int heightIn)
{
  height = heightIn;
}

void PPM::set_comment(string commentIn)
{
  comment = std::move(commentIn);
}

void PPM::close()
{
  image.close();
  cout << "File " << fileName << " saved\n";
}