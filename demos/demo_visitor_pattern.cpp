#include "visitor_pattern.h"

int main() {
  Cat cat;
  Dog dog;
  EatVisitor eat_visitor;
  SleepVisitor sleep_visitor;
  cat.Accept(eat_visitor);
  dog.Accept(eat_visitor);
  cat.Accept(sleep_visitor);
  dog.Accept(sleep_visitor);

  return 0;
}
