#include "../read_write_lock.h"
#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>

int main() {
  RWLock lock;
  auto read = [&lock]() {
    using namespace std::chrono_literals;
    lock.RLock();
    std::this_thread::sleep_for(2000ms);
    std::cout << "read finish\n";
    lock.URLock();
  };
  auto write = [&lock]() {
    using namespace std::chrono_literals;
    lock.WLock();
    std::this_thread::sleep_for(2000ms);
    std::cout << "write finish\n";
    lock.UWLock();
  };
  std::vector<std::thread> threads;
  using namespace std::chrono_literals;
  threads.push_back(std::thread(write));
  std::this_thread::sleep_for(200ms);
  threads.push_back(std::thread(read));
  threads.push_back(std::thread(read));
  std::this_thread::sleep_for(4000ms);
  threads.push_back(std::thread(write));
  threads.push_back(std::thread(write));
  std::this_thread::sleep_for(200ms);
  threads.push_back(std::thread(read));
  threads.push_back(std::thread(read));

  for_each(threads.begin(), threads.end(), [](auto &i) { i.join(); });

  return 0;
}