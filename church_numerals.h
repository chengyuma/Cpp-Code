#include <functional>

template <typename T> using Apply = std::function<T(T)>;

template <typename T> Apply<T> Zero() {
  return [](T x) { return x; };
}

template <typename T> Apply<T> AddOne(Apply<T> apply, Apply<T> num) {
  return [apply, num](T x) { return apply(num(x)); };
}

template <typename T> Apply<T> Add(Apply<T> num1, Apply<T> num2) {
  return [num1, num2](T x) { return num1(num2(x)); };
}
