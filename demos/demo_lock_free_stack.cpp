#include "../lock_free_stack.h"
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main() {
  LockFreeStack<int> stack;

  using std::chrono::operator""ms;
  const auto now = std::chrono::steady_clock::now();
  const auto start1 = now + 2000ms;
  const auto start2 = now + 3000ms;
  std::vector<std::thread> push_threads;
  std::vector<std::thread> pop_threads;
  auto push_task = [&stack](int i, const auto &start) {
    std::this_thread::sleep_until(start);
    stack.Push(i);
  };
  auto pop_task = [&stack](int i, const auto &start) {
    int *temp = new int;
    std::this_thread::sleep_until(start);
    static std::mutex mut_;
    std::unique_lock<std::mutex> io_lock(mut_);
    if (stack.Pop(temp)) {
      std::cout << "Pop success, data: " << *temp << std::endl;
    } else {
      std::cout << "Pop failed\n";
    }
  };
  for (int i = 0; i < 5; ++i) {
    push_threads.emplace_back(push_task, i, start1);
  }

  for (int i = 0; i < 5; ++i) {
    pop_threads.emplace_back(pop_task, i, start1);
  }
  for (int i = 0; i < 5; ++i) {
    pop_threads.emplace_back(pop_task, i, start2);
  }

  for (auto &i : push_threads) {
    i.join();
  }
  for (auto &i : pop_threads) {
    i.join();
  }
}
