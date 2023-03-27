#include "../producer_onsumer_model.h"

#include <iostream>


int main() {
  std::mutex mut;
  ProducerConsumerModel<int> obj;
  obj.AddProducer(
      []() {
        int temp = random();
        std::cout << "Produce: " << temp << std::endl;
        return temp;
      },
      10);
  obj.AddConsumer([](auto i) { std::cout << "Consume: " << i << std::endl; },
                  10);

  obj.JoinAllThread();

  return 0;
}