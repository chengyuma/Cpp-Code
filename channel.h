#pragma once

#include <memory>
#include <mutex>
#include <queue>
#include <condition_variable>

// like golang Channel
template <typename T> class Channel {
public:
  Channel() : is_close(false) {}
  ~Channel() = default;
  template <typename U> bool Send(U &&data);
  bool Recive(T *data_ptr);
  void Close();

private:
  std::queue<T> q;
  std::mutex mut_;
  std::condition_variable cond_;
  bool is_close;
};

template <typename T> template <typename U> bool Channel<T>::Send(U &&data) {
  bool notify;
  {
    std::unique_lock<std::mutex> lock(mut_);
    if (is_close) {
      return false;
    }
    notify = q.empty();
    q.push(std::forward<U>(data));
  }
  if (notify) {
    cond_.notify_one();
  }
  return true;
}

template <typename T> bool Channel<T>::Recive(T *data_ptr) {
  std::unique_lock<std::mutex> lock(mut_);
  cond_.wait(lock, [this]() { return !q.empty() || is_close; });
  if (is_close) {
    return false;
  }
  *data_ptr = q.front();
  q.pop();
  return true;
}

template <typename T> void Channel<T>::Close() {
  std::unique_lock<std::mutex> lock(mut_);
  is_close = true;
  cond_.notify_all();
}
