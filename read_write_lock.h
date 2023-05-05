#pragma once

#include <mutex>
#include <condition_variable>

// could use std::shared_mutex instead
class RWLock {
public:
  RWLock() : lock_count(0), waiter(0) {}
  void RLock();
  void URLock();
  void WLock();
  void UWLock();

private:
  std::mutex mut_;
  std::condition_variable cond_;
  int lock_count; // -1, W lock; >0, R lock
  int waiter;
};

void RWLock::RLock() {
  std::unique_lock<std::mutex> lock(mut_);
  waiter++;
  if (lock_count == -1) {
    cond_.wait(lock, [this]() { return lock_count >= 0; });
  }
  lock_count++;
  waiter--;
}

void RWLock::URLock() {
  std::unique_lock<std::mutex> lock(mut_);
  if (lock_count > 0) {
    lock_count--;
    if (waiter > 0 && lock_count == 0) {
      cond_.notify_one();
    }
  } else {
    exit(1);
  }
}

void RWLock::WLock() {
  std::unique_lock<std::mutex> lock(mut_);
  waiter++;
  if (lock_count != 0) {
    cond_.wait(lock, [this]() { return lock_count == 0; });
  }
  lock_count = -1;
  waiter--;
}

void RWLock::UWLock() {
  std::unique_lock<std::mutex> lock(mut_);
  if (lock_count == -1) {
    lock_count = 0;
    if (waiter > 0) {
      cond_.notify_all();
    }
  } else {
    exit(1);
  }
}
