#include "ThreadPool.h"

#include <thread>
#include <iostream>
#include <sstream>

using namespace std;

int main()
{
  srand(0);
  auto begin = chrono::high_resolution_clock::now();

  auto numThreads = thread::hardware_concurrency();

  static std::exception_ptr globalExceptionPtr = nullptr;

  try
  {
    ThreadPool pool(numThreads);

    cout << "queueing up some random work\n";
    for (size_t i = 0; i < 1000; i++)
    {
      // pass in lambda of some random work
      pool.enqueue_work([i]()
                        {
                          try
                          {
                            size_t x;
                            size_t repetitions = 10 + (10 * (rand() % 5));
                            for (size_t j = 0; j < repetitions; j++)
                            {
                              x = i + (rand() % 200); // pretty good chance of generating a few
                              // exceptions every few runs
                            }
                            if (x > 1000)
                            {
                              stringstream id;
                              id << this_thread::get_id();
                              throw runtime_error(id.str().c_str());
                            }
                          } catch (...)
                          {
                            globalExceptionPtr = current_exception();
                          }
                        });
      if (globalExceptionPtr)
      {
        try
        {
          rethrow_exception(globalExceptionPtr);
        } catch (const exception &e)
        {
          cout << "caught exception in job from thread: " << e.what() << "\n";
        }
      }
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - begin);
    cout << "pool duration = " << duration.count() / 1000.f << " seconds\n";
  } catch (const exception &ex)
  {
    cout << "caught exception from thread pool: " << ex.what() << "\n";
  }

  return 0;
}