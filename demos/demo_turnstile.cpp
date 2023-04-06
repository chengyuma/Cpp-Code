#include "../turnstile.h"

#include <chrono>
#include <iostream>
#include <thread>

int main() {
  TurnstileLock t_lock;

  std::thread th1([&t_lock]() {
    t_lock.Lock();
    std::cout << "th1: t_lock.Lock()\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    t_lock.UnLock();
    std::cout << "th1: t_lock.UnLock()\n";
  });

  std::thread th2([&t_lock]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    t_lock.Lock();
    std::cout << "th2: t_lock.Lock()\n";
    t_lock.UnLock();
    std::cout << "th2: t_lock.UnLock()\n";
  });

  th1.join();
  th2.join();

  return 0;
}
