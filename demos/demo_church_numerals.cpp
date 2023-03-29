#include "../church_numerals.h"

#include <iostream>

int main() {
  auto apply = [](int i) { return i + 1; };

  auto zero = Zero<int>();
  auto one = AddOne<int>(apply, AddOne<int>(apply, zero));

  std::cout << one(0) << std::endl;

  return 0;
}