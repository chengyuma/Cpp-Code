#include "../read_write_lock.h"
#include <iostream>
#include <thread>

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
  using namespace std::chrono_literals;
  std::thread th1(write);
  std::this_thread::sleep_for(200ms);
  std::thread th2(read);
  std::thread th3(read);
  std::this_thread::sleep_for(4000ms);
  std::thread th4(write);
  std::thread th5(write);
  std::this_thread::sleep_for(200ms);
  std::thread th6(read);
  std::thread th7(read);

  th1.join();
  th2.join();
  th3.join();
  th4.join();
  th5.join();
  th6.join();
  th7.join();

  return 0;
}