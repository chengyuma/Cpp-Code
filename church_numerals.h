#include <functional>

template <typename T> using Apply = std::function<T(T)>;

template <typename T> Apply<T> Zero() {
  return [](T x) { return x; };
}

template <typename T>
Apply<T> AddOne(const Apply<T> &apply, const Apply<T> &num) {
  return [&apply, &num](T x) { return apply(num(x)); };
}
