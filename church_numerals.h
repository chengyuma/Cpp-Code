#pragma once

#include <functional>

template <typename T> using Apply = std::function<T(T)>;

template <typename T> Apply<T> Zero() {
  return [](T x) { return x; };
}

template <typename T>
Apply<T> AddOne(const Apply<T> &apply, const Apply<T> &num) {
  return [apply, num](T x) { return apply(num(x)); };
}

template <typename T> Apply<T> Add(const Apply<T> &num1, const Apply<T> &num2) {
  return [num1, num2](T x) { return num1(num2(x)); };
}
