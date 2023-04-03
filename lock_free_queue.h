// #pragma once

#include <atomic>
#include <chrono>
#include <memory>

template <typename T> struct Node {
  T data;
  std::shared_ptr<Node> next;
  std::atomic<bool> flag_;

  template <typename U>
  Node(U &&data) : data(std::forward<U>(data)), flag_(false) {}
};

template <typename T> class LockFreeQueue {
public:
  template <typename... ArgsOfTsConstructor>
  LockFreeQueue(ArgsOfTsConstructor &&...args) {
    // make a dummy node
    head = std::make_shared<Node<T>>(
        T(std::forward<ArgsOfTsConstructor>(args)...));
    tail = head;
  };
  LockFreeQueue(const LockFreeQueue &) = delete;
  LockFreeQueue(LockFreeQueue &&) = delete;
  LockFreeQueue &operator=(const LockFreeQueue &) = delete;
  LockFreeQueue &operator=(LockFreeQueue &&) = delete;

  template <typename U> void Push(U &&data);
  bool Pop(T *data);

private:
  std::shared_ptr<Node<T>> head;
  std::shared_ptr<Node<T>> tail;
};

template <typename T>
template <typename U>
void LockFreeQueue<T>::Push(U &&data) {
  auto new_node = std::make_shared<Node<T>>(std::forward<U>(data));
  std::shared_ptr<Node<T>> temp;
  auto old_tail = std::atomic_load(&tail);
  while (
      old_tail->flag_.load(std::memory_order_acquire) ||
      !std::atomic_compare_exchange_weak(&(old_tail->next), &temp, new_node)) {
    temp.reset();
    old_tail = std::atomic_load(&tail);
  }
  std::atomic_compare_exchange_weak(&tail, &old_tail, new_node);
}

template <typename T> bool LockFreeQueue<T>::Pop(T *data) {
  std::shared_ptr<Node<T>> popped;
  while (true) {
    popped = std::atomic_load(&(head->next));
    if (popped) {
      popped->flag_.exchange(true);
      if (std::atomic_compare_exchange_weak(&(head->next), &popped,
                                            popped->next)) {
        *data = popped->data;
        std::atomic_compare_exchange_weak(&tail, &popped, head);
        return true;
      } else {
        popped->flag_.exchange(false);
      }
    }
  }
}
