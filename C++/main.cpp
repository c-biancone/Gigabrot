#include "PPM.h"
#include "Colorization.h"
#include "Mandelbrot.h"
#include "ThreadPool.h"

#include <chrono>
#include <iostream>
#include <vector>

using namespace std;

int main()
{

  int width;
  int height;
  cout << "Enter width and height in pixels:\n";
  cin >> width >> height;

  string fileName;
  cout << "Enter output file name in the form 'name.ppm':\n";
  cin >> fileName;

  auto begin = chrono::steady_clock::now();

  // set up image stream for writing
  PPM pgm(fileName, width, height);
  // PPM pgm(width, height); // for testing
  if (!pgm.init_stream())
  {
    cout << "Could not open ofstream for image\n";
  }
  pgm.write_header();

  // set up container for image row data
  vector<unsigned char> row{}; // didn't realize array needs compile-time const length :(
  row.resize(width * 3); // avoid constant resizing by allocating up front

  cout << "Rendering row by row:\n";

  Mandelbrot gigabrot(width, height);
  cout << gigabrot;

  unsigned int numThreads = thread::hardware_concurrency();
  // cout << "numThreads: " << numThreads << "\n";
  for (size_t pY = 0; pY < height; pY++)
  {
    //mutex mtx;
    //scoped_lock guard(mtx);
// #pragma omp parallel for schedule(dynamic)
    for (size_t pX = 0; pX < width; pX++)
    {
      //Mandelbrot newBrot(width, height);
      size_t subPixel = 3 * pX;
      gigabrot.current_pixel(pX, pY);
      gigabrot.get_c();
      gigabrot.iterate();
      row[subPixel + 2] = row[subPixel + 1] = row[subPixel] =
          gigabrot.colorize_bw();
      gigabrot.reset();
    }
    {
      // implemented due to possibility of having huge image, keep memory usage low
      pgm.write_row(row);
    }
  }

  pgm.close();

  auto end = chrono::steady_clock::now();
  cout << "Time elapsed:"
       << chrono::duration_cast<chrono::seconds>(end - begin).count()
       << " sec\n";
  return 0;
}