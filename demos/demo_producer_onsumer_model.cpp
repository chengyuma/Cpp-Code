#include "../producer_onsumer_model.h"

#include <iostream>

int main() {
  std::mutex mut;
  ProducerConsumerModel<int> obj;
  obj.AddProducer(
      []() {
        int temp = random();
        std::cout << "Produce: " << temp << std::endl;  // std::cout is not thread safe
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1000ms);
        return temp;
      },
      10);
  obj.AddConsumer(
      [](auto i) {
        std::cout << "Consume: " << i << std::endl;
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(500ms);
      },
      10);

  obj.JoinAllThread();

  return 0;
}