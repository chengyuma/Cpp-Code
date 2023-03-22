#include <iostream>
#include <queue>
#include <thread>

template <typename T> class Chanel {
public:
  Chanel() : is_close(false) {}
  ~Chanel() = default;
  template <typename U> bool Send(U &&data);
  bool Recive(T *data_ptr);
  void Close();

private:
  std::queue<T> q;
  std::mutex mut_;
  std::condition_variable cond_;
  bool is_close;
};

template <typename T> template <typename U> bool Chanel<T>::Send(U &&data) {
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

template <typename T> bool Chanel<T>::Recive(T *data_ptr) {
  std::unique_lock<std::mutex> lock(mut_);
  cond_.wait(lock, [this]() { return !q.empty() || is_close; });
  if (is_close) {
    return false;
  }
  *data_ptr = std::move(q.front());
  q.pop();
  return true;
}

template <typename T> void Chanel<T>::Close() {
  std::unique_lock<std::mutex> lock(mut_);
  is_close = true;
  cond_.notify_all();
}

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
