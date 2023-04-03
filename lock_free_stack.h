#pragma once

#include <atomic>
#include <memory>

// define WAIT_DATA: if there is no data in stack, wait data.
// no define WAIT_DATA: if there is no data in stack, return false.
#define WAIT_DATA
template <typename T> struct Node {
  T data;
  std::shared_ptr<Node> next;
  template <typename U> Node(U &&data) : data(std::forward<U>(data)) {}
};

template <typename T> class LockFreeStack {
public:
  LockFreeStack() = default;
  LockFreeStack(const LockFreeStack &) = delete;
  LockFreeStack(LockFreeStack &&) = delete;

  template <typename U> void Push(U &&data);
  bool Pop(T *data);

private:
  std::shared_ptr<Node<T>> head;
};

template <typename T>
template <typename U>
void LockFreeStack<T>::Push(U &&data) {
  auto new_node = std::make_shared<Node<T>>(std::forward<U>(data));
  new_node->next = std::atomic_load(&head);
  while (
      !std::atomic_compare_exchange_weak(&head, &(new_node->next), new_node)) {
    ;
  }
}

#ifdef WAIT_DATA
template <typename T> bool LockFreeStack<T>::Pop(T *data) {
  std::shared_ptr<Node<T>> popped;
  while (!std::atomic_compare_exchange_weak(&head, &popped,
                                            popped ? popped->next : nullptr) ||
         popped == nullptr) {
    ;
  }
  *data = popped->data;
  return true;
}
#endif

#ifndef WAIT_DATA
template <typename T> bool LockFreeStack<T>::Pop(T *data) {
  std::shared_ptr<Node<T>> popped(std::atomic_load(&head));
  while (popped &&
         !std::atomic_compare_exchange_weak(&head, &popped, popped->next)) {
    ;
  }
  if (popped == nullptr) {
    return false;
  }
  *data = popped->data;
  return true;
}
#endif
