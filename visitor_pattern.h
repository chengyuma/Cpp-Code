// a simple example of visitor pattern using animals to demonstrate

#include <iostream>
#include <string>

class Cat;
class Dog;

class AnimalVisitor {
public:
  virtual void Visit(Cat &) = 0;
  virtual void Visit(Dog &) = 0;
};

class EatVisitor : public AnimalVisitor {
public:
  virtual void Visit(Cat &cat) override { std::cout << "Cat eat.\n"; }
  virtual void Visit(Dog &dog) override { std::cout << "Dog eat.\n"; }
};

class SleepVisitor : public AnimalVisitor {
public:
  void Visit(Cat &cat) { std::cout << "Cat sleep.\n"; }
  void Visit(Dog &dog) { std::cout << "Dog sleep.\n"; }
};

class Animal {
public:
  virtual void Accept(AnimalVisitor &Visitor) = 0;
};

class Cat : public Animal {
public:
  virtual void Accept(AnimalVisitor &Visitor) override { Visitor.Visit(*this); }
};

class Dog : public Animal {
public:
  virtual void Accept(AnimalVisitor &Visitor) override { Visitor.Visit(*this); }
};
