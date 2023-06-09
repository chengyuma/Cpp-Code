#pragma once

#include <mutex>
#include <queue>
#include <condition_variable>

// like turnstile in Solaris

struct Turnstile {
  std::mutex mut_;
  std::condition_variable cond_;
  bool could_pass;
  int waiting_count;
  Turnstile() : could_pass(true), waiting_count(0) {}
};

class TurnstilePool {
public:
  TurnstilePool(const TurnstilePool &) = delete;
  TurnstilePool(const TurnstilePool &&) = delete;
  TurnstilePool& operator=(const TurnstilePool &) = delete;
  TurnstilePool& operator=(const TurnstilePool &&) = delete;

  static std::shared_ptr<Turnstile> Get();
  static void Put(std::shared_ptr<Turnstile> &&ptr);

private:
  inline static int used_turnstile = 0;
  inline static std::queue<std::shared_ptr<Turnstile>> queue;
  inline static std::mutex mut;
  TurnstilePool() = default;
};

class TurnstileLock {
public:
  TurnstileLock() = default;
  TurnstileLock(const TurnstileLock &) = delete;
  TurnstileLock(const TurnstileLock &&) = delete;
  TurnstileLock& operator=(const TurnstileLock &) = delete;
  TurnstileLock& operator=(const TurnstileLock &&) = delete;

  void Lock();
  void UnLock();

private:
  std::shared_ptr<Turnstile> turnstile;
};

std::shared_ptr<Turnstile> TurnstilePool::Get() {
  std::unique_lock<std::mutex> lock(mut);
  used_turnstile++;
  if (queue.size() > 0) {
    auto ret = std::move(queue.front());
    queue.pop();
    return ret;
  } else {
    return std::make_shared<Turnstile>();
  }
}

void TurnstilePool::Put(std::shared_ptr<Turnstile> &&ptr) {
  std::unique_lock<std::mutex> lock(mut);
  used_turnstile--;
  queue.push(std::move(ptr));
}

void TurnstileLock::Lock() {
  if (turnstile == nullptr) {
    turnstile = TurnstilePool::Get();
  }
  std::unique_lock<std::mutex> lock(turnstile->mut_);
  (turnstile->waiting_count)++;
  if (turnstile->waiting_count == 1) {
    turnstile->could_pass = false;
  } else {
    turnstile->cond_.wait(lock, [this] { return turnstile->could_pass; });
  }
}

void TurnstileLock::UnLock() {
  std::unique_lock<std::mutex> lock(turnstile->mut_);
  turnstile->waiting_count--;
  if (turnstile->waiting_count == 0) {
    TurnstilePool::Put(std::move(turnstile));
  } else {
    turnstile->could_pass = true;
    turnstile->cond_.notify_one();
  }
}
