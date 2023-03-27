#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

template <typename T> class ProducerConsumerModel {
public:
  ProducerConsumerModel() : ready(false) {}

  void AddProducer(std::function<T(void)> fun, int times);
  void AddConsumer(std::function<void(T)> fun, int times);
  void JoinAllThread();

private:
  std::mutex mut;
  std::condition_variable consumer_cond;
  std::condition_variable producer_cond;
  std::vector<std::thread> producers;
  std::vector<std::thread> consumers;
  T buffer;
  bool ready;
};

template <typename T>
void ProducerConsumerModel<T>::AddProducer(std::function<T(void)> fun,
                                           int times) {
  std::thread th([this, fun, times]() {
    for (int i = 0; i < times; ++i) {
      {
        std::unique_lock<std::mutex> lock(mut);
        producer_cond.wait(lock, [this]() { return !ready; });
        buffer = fun();
        ready = true;
        consumer_cond.notify_all();
      }
    }
  });
  producers.push_back(std::move(th));
}

template <typename T>
void ProducerConsumerModel<T>::AddConsumer(std::function<void(T)> fun,
                                           int times) {
  std::thread th([this, fun, times]() {
    for (int i = 0; i < times; ++i) {
      T my_buffer;
      {
        std::unique_lock<std::mutex> lock(mut);
        consumer_cond.wait(lock, [this]() { return ready; });
        my_buffer = std::move(buffer);
        ready = false;
        producer_cond.notify_all();
      }
      fun(my_buffer);
    }
  });
  producers.push_back(std::move(th));
}

template <typename T> void ProducerConsumerModel<T>::JoinAllThread() {
  std::for_each(producers.begin(), producers.end(), [](auto &i) { i.join(); });
  std::for_each(consumers.begin(), consumers.end(), [](auto &i) { i.join(); });
}