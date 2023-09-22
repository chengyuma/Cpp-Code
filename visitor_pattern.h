// a simple example of visitor pattern using animals to demonstrate

#pragma once

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
  virtual void Visit(Cat &cat) override { std::cout << "Cat is eating.\n"; }
  virtual void Visit(Dog &dog) override { std::cout << "Dog is eating.\n"; }
};

class SleepVisitor : public AnimalVisitor {
public:
  virtual void Visit(Cat &cat) override { std::cout << "Cat is sleeping.\n"; }
  virtual void Visit(Dog &dog) override { std::cout << "Dog is sleeping.\n"; }
};

class Animal {
public:
  virtual void Accept(AnimalVisitor &Visitor) = 0;
};

class Cat : public Animal {
public:
  virtual void Accept(AnimalVisitor &visitor) override { visitor.Visit(*this); }
};

class Dog : public Animal {
public:
  virtual void Accept(AnimalVisitor &visitor) override { visitor.Visit(*this); }
};
