#include "../producer_onsumer_model.h"

#include <iostream>

int main() {
  std::mutex mut;
  ProducerConsumerModel<int> obj;
  for (int i = 0; i < 10; i++) {
    obj.AddProducer([]() {
      int temp = random();
      // std::cout is not thread safe
      std::cout << "Produce: " << temp << std::endl;
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(1000ms);
      return temp;
    });
  }

  for (int i = 0; i < 10; i++) {
    obj.AddConsumer([](auto i) {
      std::cout << "Consume: " << i << std::endl;
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(500ms);
    });
  }

  obj.JoinAllThread();

  return 0;
}