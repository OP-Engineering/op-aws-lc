#pragma once

#include <condition_variable>
#include <exception>
#include <mutex>
#include <queue>
#include <stdio.h>
#include <thread>
#include <vector>

namespace opawslc {

class ThreadPool {
public:
  static ThreadPool &getInstance() {
    static ThreadPool instance; // Initialized once, thread-safe since C++11
    return instance;
  }

  void queueWork(const std::function<void(void)> &task);
  void waitFinished();
  void restartPool();

private:
  ThreadPool();  // Private constructor
  ~ThreadPool(); // Private destructor to prevent external deletion

  ThreadPool(const ThreadPool &) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;

  unsigned int busy{};
  std::condition_variable_any workQueueConditionVariable;
  std::vector<std::thread> threads;
  std::mutex workQueueMutex;
  std::queue<std::function<void(void)>> workQueue;
  bool done;

  void doWork();
};

} // namespace opawslc