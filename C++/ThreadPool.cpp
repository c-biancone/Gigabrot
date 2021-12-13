#include "ThreadPool.h"

ThreadPool::ThreadPool(unsigned int numThreads) : queues(numThreads), count(numThreads)
{
  if (!numThreads)
  {
    throw std::invalid_argument("thread count must be nonzero!\n");
  } else if (numThreads < 0) {
    throw std::invalid_argument("thread count must be positive! how did this happen??");
  }

  auto worker = [this] (auto i) {
    while (true)
    {
      process proc;
      for (auto j = 0; j < count * countMult; j++)
      {
        if (queues[(i + j) % count].try_pop(proc))
        {
          break;
        }
      }
      if (!proc && !queues[i].pop(proc))
      {
        break;
      }
      proc();
    }
  };

  threads.reserve(numThreads);

  for (auto i = 0; i < numThreads; i++)
  {
    threads.emplace_back(worker, i);
  }
}

ThreadPool::~ThreadPool()
{
  for (auto& queue: queues)
  {
    queue.unblock();
  }
  for (auto& thread : threads)
  {
    thread.join();
  }
}