#include "church_numerals.h"

#include <iostream>

int main() {
  auto apply = [](int i) { return i + 1; };

  auto zero = Zero<int>();
  auto one = AddOne<int>(apply, zero);
  auto two = AddOne<int>(apply, AddOne<int>(apply, zero));
  auto ten = Zero<int>();
  for (int i = 0; i < 10; i++) {
    ten = AddOne<int>(apply, ten);
  }
  auto twelve = Add(two, ten);

  std::cout << zero(0) << std::endl;
  std::cout << one(0) << std::endl;
  std::cout << two(0) << std::endl;
  std::cout << ten(0) << std::endl;
  std::cout << twelve(0) << std::endl;

  return 0;
}