#ifndef C____THREADPOOL_H_
#define C____THREADPOOL_H_

#include "UnboundedQueue.h"

#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <atomic>
#include <future>

class ThreadPool
{
 public:

  explicit ThreadPool(unsigned int numThreads);

  ~ThreadPool();

  template<typename T, typename... ARGS>
  void enqueue_work(T&& t, ARGS&&... args)
  {
    auto work = [proc = std::forward<T>(t), tuple = std::make_tuple(std::forward<ARGS>(args)...)]
        () {std::apply(proc, tuple);};

    auto i = index++;

    for (auto j = 0; j < count * countMult; j++)
    {
      if (queues[(i + j) % count].try_push(work))
      {
        return;
      }
    }

    queues[i % count].push(std::move(work));
  }

  template<typename T, typename... ARGS>
  void enqueue_task(T&& t, ARGS&&... args)
  {
    using taskReturnType = std::invoke_result<T, ARGS...>;
    using taskType = std::packaged_task<taskReturnType()>;

    auto task = std::make_shared<taskType>(std::bind(std::forward<T>(t), std::forward<ARGS>(args)...));
    auto work = [=] () {(*task)();};
    auto result = task->get_future();

    auto i = index++;

    for (auto j = 0; j < count * countMult; j++)
    {
      if (queues[(i + j) % count].try_push(work))
      {
        return result;
      }
    }

    queues[i % count].push(std::move(work));

    return result;
  }

 private:

  using process = std::function<void(void)>;

  using queue = UnboundedQueue<process>;

  using queueVec = std::vector<queue>;

  queueVec queues;


  using Threads = std::vector<std::thread>;

  Threads threads;


  const std::size_t count;

  std::atomic_uint index = 0;

  inline static const unsigned int countMult = 2;

};

#endif //C____THREADPOOL_H_
