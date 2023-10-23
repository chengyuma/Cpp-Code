#include "producer_consumer_model.h"

#include <iostream>
#include <mutex>

std::mutex io_mut;

int main() {
  std::mutex mut;
  ProducerConsumerModel<int> obj(5);
  for (int i = 0; i < 10; i++) {
    obj.AddProducer([]() {
      int temp = random();
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(1000ms);
      {
        std::unique_lock<std::mutex> lock(io_mut);
        std::cout << "Produce: " << temp << std::endl;
      }
      return temp;
    });
  }

  for (int i = 0; i < 10; i++) {
    obj.AddConsumer([](auto i) {
      {
        std::unique_lock<std::mutex> lock(io_mut);
        std::cout << "Consume: " << i << std::endl;
      }
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(500ms);
    });
  }

  obj.JoinAllThread();

  return 0;
}