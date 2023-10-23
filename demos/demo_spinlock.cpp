#include "spinlock.h"
#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>

int main() {
  SpinLock lock;
  auto fun = [&lock](int i) {
    for (int j = 0; j < 10; ++j) {
      lock.Lock();
      std::cout << j << ' ';
      lock.Unlock();
    }
  };
  std::vector<std::thread> v;
  for (int i = 0; i < 5; ++i) {
    v.emplace_back(fun, i);
  }
  std::for_each(v.begin(), v.end(), [](auto &i) { i.join(); });
  return 0;
}
