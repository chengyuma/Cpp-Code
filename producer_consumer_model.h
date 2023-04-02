#pragma once

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <thread>
#include <utility>

template <typename T> class ProducerConsumerModel {
public:
  explicit ProducerConsumerModel(size_t buffer_size)
      : buffer_size(buffer_size), used_buffer_size(0), begin(0), end(0) {
    buffer.resize(buffer_size);
  }

  void AddProducer(const std::function<T()> &fun);
  void AddConsumer(const std::function<void(T)> &fun);
  void JoinAllThread();

private:
  size_t updateIndex(size_t index) { return (index + 1) % buffer_size; }

  std::mutex mut;
  std::condition_variable consumer_cond;
  std::condition_variable producer_cond;
  std::vector<std::thread> producers;
  std::vector<std::thread> consumers;
  std::vector<T> buffer;
  size_t buffer_size;
  size_t used_buffer_size;
  size_t begin;
  size_t end;
};

template <typename T>
void ProducerConsumerModel<T>::AddProducer(const std::function<T()> &fun) {
  std::thread th([this, fun]() {
    std::unique_lock<std::mutex> lock(mut);
    producer_cond.wait(lock,
                       [this]() { return used_buffer_size != buffer_size; });
    buffer[end] = fun();
    used_buffer_size++;
    end = updateIndex(end);
    consumer_cond.notify_one();
  });

  producers.push_back(std::move(th));
}

template <typename T>
void ProducerConsumerModel<T>::AddConsumer(const std::function<void(T)> &fun) {
  std::thread th([this, fun]() {
    T my_buffer;
    {
      std::unique_lock<std::mutex> lock(mut);
      consumer_cond.wait(lock, [this]() { return used_buffer_size > 0; });
      my_buffer = std::move(buffer[begin]);
      used_buffer_size--;
      begin = updateIndex(begin);
      producer_cond.notify_one();
    }
    fun(my_buffer);
  });
  producers.push_back(std::move(th));
}

template <typename T> void ProducerConsumerModel<T>::JoinAllThread() {
  std::for_each(producers.begin(), producers.end(), [](auto &i) { i.join(); });
  std::for_each(consumers.begin(), consumers.end(), [](auto &i) { i.join(); });
}
