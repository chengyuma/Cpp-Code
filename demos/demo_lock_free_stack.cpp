#include "../lock_free_stack.h"
#include <iostream>

int main() {
  LockFreeStack<int> stack;

  for (int i = 0; i < 4; i++) {
    stack.Push(i);
  }
  for (int i = 0; i < 5; i++) {
    int *temp = new int;
    if (stack.Pop(temp)) {
      std::cout << "Pop success, data: " << *temp << std::endl;
    } else {
      std::cout << "Pop faild\n";
    }
  }
}
