#include "../chanel.h"
#include <iostream>
#include <queue>
#include <thread>

int main() {
  Chanel<int> Ch;
  std::thread th([&Ch]() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(200ms);
    Ch.Send(1);
  });
  th.join();
  auto p = std::make_shared<int>(0);
  Ch.Recive(p.get());
  Ch.Close();
  std::cout << *p << std::endl;

  return 0;
}
