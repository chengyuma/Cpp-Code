#include "lru_cache.h"
#include <iostream>

int main() {
  LRUCache<int, int> cache(2);
  int *val_ptr = new int;
  cache.Put(1, 1);
  cache.Put(2, 2);
  if (cache.Get(1, val_ptr)) {
    std::cout << "true: " << *val_ptr << std::endl;
  } else {
    std::cout << "false\n";
  }
  cache.Put(3, 3);
  if (cache.Get(2, val_ptr)) {
    std::cout << "true: " << *val_ptr << std::endl;
  } else {
    std::cout << "false\n";
  }
  cache.Put(4, 4);
  if (cache.Get(1, val_ptr)) {
    std::cout << "true: " << *val_ptr << std::endl;
  } else {
    std::cout << "false\n";
  }
  if (cache.Get(3, val_ptr)) {
    std::cout << "true: " << *val_ptr << std::endl;
  } else {
    std::cout << "false\n";
  }
  if (cache.Get(4, val_ptr)) {
    std::cout << "true: " << *val_ptr << std::endl;
  } else {
    std::cout << "false\n";
  }
}