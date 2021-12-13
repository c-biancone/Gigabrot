#ifndef C____UNBOUNDEDQUEUE_H_
#define C____UNBOUNDEDQUEUE_H_

#include <mutex>
#include <queue>
#include <utility>
#include <stdexcept>
#include <condition_variable>

template<typename T>
class UnboundedQueue
{
 public:

  /**
   * Default Parametrized constructor
   * @param block
   */
  explicit UnboundedQueue(bool block = true);

  /**
   * Default deconstructor
   */
  ~UnboundedQueue();

  /**
   * @param item - const reference
   */
  void push(const T& item);

  /**
   * @param item - double reference
   */
  void push(T&& item);

  /**
   * Emplace
   * @tparam Args
   * @param args
   */
  template<typename... ARGS>
  void emplace(ARGS&&... args);

  bool try_push(const T& item);

  bool try_push(T&& item);

  bool pop(T& item);

  bool try_pop(T& item);

  std::size_t size() const;

  bool empty() const;

  void block();

  void unblock();

  bool blocking() const;


 private:

  using queue_t = std::queue<T>;
  queue_t queue;

  bool is_block;

  mutable std::mutex queueLock;

  std::condition_variable condition;

};

template<typename T>
UnboundedQueue<T>::UnboundedQueue(bool block) : is_block(block)
{}

template<typename T>
void UnboundedQueue<T>::push(const T &item)
{
  {
    std::scoped_lock guard(queueLock);
    queue.push(item);
  }
  condition.notify_one();
}

template<typename T>
void UnboundedQueue<T>::push(T &&item)
{
  {
    std::scoped_lock guard(queueLock);
    queue.push(std::move(item));
  }
  condition.notify_one();
}

template<typename T>
template<typename... ARGS>
void UnboundedQueue<T>::emplace(ARGS &&... args)
{
  {
    std::scoped_lock guard(queueLock);
    queue.emplace(std::forward<>(args)...);
  }
  condition.notify_one();
}

template<typename T>
bool UnboundedQueue<T>::try_push(const T &item)
{
  {
    std::unique_lock guard(queueLock, std::try_to_lock);
    if (!guard)
    {
      return false;
    }
    queue.push(item);
  }
  condition.notify_one();
  return true;
}

template<typename T>
bool UnboundedQueue<T>::try_push(T &&item)
{
  {
    std::unique_lock guard(queueLock, std::try_to_lock);
    if (!guard)
    {
      return false;
    }
    queue.push(std::move(item));
  }
  condition.notify_one();
  return true;
}

template<typename T>
bool UnboundedQueue<T>::pop(T &item)
{
  std::unique_lock guard(queueLock);
  condition.wait(guard, [&] () {return !queue.empty() || !is_block;});
  if (queue.empty())
  {
    return false;
  }
  item = std::move(queue.front());
  queue.pop();
  return true;
}

template<typename T>
bool UnboundedQueue<T>::try_pop(T &item)
{
  std::unique_lock guard(queueLock, std::try_to_lock);
  if (!guard || queue.empty())
  {
    return false;
  }
  item = std::move(queue.front());
  queue.pop();
  return true;
}

template<typename T>
std::size_t UnboundedQueue<T>::size() const
{
  std::scoped_lock guard(queueLock);
  return queue.size();
}

template<typename T>
bool UnboundedQueue<T>::empty() const
{
  std::scoped_lock guard(queueLock);
  return queue.empty();
}

template<typename T>
void UnboundedQueue<T>::block()
{
  std::scoped_lock guard(queueLock);
  is_block = true;
}

template<typename T>
void UnboundedQueue<T>::unblock()
{
  {
    std::scoped_lock guard(queueLock);
    is_block = false;
  }
  condition.notify_all();
}

template<typename T>
bool UnboundedQueue<T>::blocking() const
{
  std::scoped_lock guard(queueLock);
  return is_block;
}

template<typename T>
UnboundedQueue<T>::~UnboundedQueue()
= default;



#endif //C____UNBOUNDEDQUEUE_H_