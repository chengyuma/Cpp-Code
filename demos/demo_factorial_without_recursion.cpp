#include "../factorial_without_recursion.h"
#include <iostream>

int main() {
  std::cout << FactorialSimple(10) << std::endl;
  std::cout << FactorialWithTailRecursion(10, 1) << std::endl;
  auto fun = []() {
    return ResultOrFunction(FactorialWithoutRecursion(10, 1));
  };
  std::cout << Eval(fun) << std::endl;
}