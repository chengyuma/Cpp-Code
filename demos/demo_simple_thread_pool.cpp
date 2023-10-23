#include "simple_thread_pool.h"
#include <chrono>
#include <iostream>

int main() {
  SimpleThreadPool thread_pool(3);
  thread_pool.Start();
  auto task = [](int i) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    static std::mutex mut;
    std::unique_lock<std::mutex> io_lock(mut);
    std::cout << "finish " << i << std::endl;
  };
  for (int i = 0; i < 10; ++i) {
    thread_pool.PushTask(task, i);
  }
  std::this_thread::sleep_for(std::chrono::seconds(1));
  thread_pool.Stop(false);

  std::cout << std::string(20, '-') << std::endl;
  thread_pool.Start();
  for (int i = 0; i < 10; ++i) {
    thread_pool.PushTask(task, i);
  }
  thread_pool.Stop(true);

  std::cout << std::string(20, '-') << std::endl;
  auto task_with_ret = [](int i) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    static std::mutex mut;
    std::unique_lock<std::mutex> io_lock(mut);
    std::cout << "finish " << i << std::endl;
    return -i;
  };
  thread_pool.Start();
  std::vector<std::future<int>> future_vec;
  for (int i = 0; i < 10; ++i) {
    future_vec.push_back(
        std::move(thread_pool.PushTaskWithRetrunValue(task_with_ret, i)));
  }
  thread_pool.Stop(true);
  for (int i = 0; i < 10; ++i) {
    std::cout << "i: " << i << ", future: " << future_vec[i].get() << std::endl;
  }

  return 0;
}
