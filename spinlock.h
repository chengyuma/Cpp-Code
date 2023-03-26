#include <atomic>

class SpinLock {
public:
  SpinLock() : lock(ATOMIC_FLAG_INIT) {}

  void Lock();
  void Unlock();

private:
  std::atomic_flag lock;
};

void SpinLock::Lock() {
  while (lock.test_and_set(std::memory_order_acquire)) {
    ;
  }
}

void SpinLock::Unlock() { lock.clear(std::memory_order_release); }
