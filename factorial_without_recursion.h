#pragma once

#include <any>
#include <functional>
#include <variant>


// simple recursive factorial
int FactorialSimple(int i) {
  if (i == 1) {
    return 1;
  } else {
    return i * FactorialSimple(i - 1);
  }
}

// recursive factorial with tail recursion
int FactorialWithTailRecursion(int i, int partial_product) {
  if (i == 1) {
    return partial_product;
  } else {
    return FactorialWithTailRecursion(i - 1, partial_product * i);
  }
}

// factorial without tail recursion
class ResultOrFunction {
public:
  explicit ResultOrFunction(int val) : ret(val) {}
  explicit ResultOrFunction(std::function<ResultOrFunction()> f) : ret(f) {}
  bool IsResult() { return ret.index() == 0; };
  bool IsFunction() { return ret.index() == 1; };
  std::optional<int> GetResult() {
    if (ret.index() == 0) {
      return get<0>(ret);
    } else {
      return {};
    }
  }
  std::optional<std::function<ResultOrFunction()>> GetFunction() {
    if (ret.index() == 1) {
      return get<1>(ret);
    } else {
      return {};
    }
  }

private:
  std::variant<int, std::function<ResultOrFunction()>> ret;
};

int Eval(std::function<ResultOrFunction()> f) {
  auto fun = std::move(f);
  while (true) {
    auto ret = fun();
    if (ret.IsResult()) {
      return ret.GetResult().value();
    } else {
      fun = ret.GetFunction().value();
    }
  }
}

ResultOrFunction FactorialWithoutRecursion(int i, int partial_product) {
  if (i == 1) {
    return ResultOrFunction(partial_product);
  } else {
    auto fun = [i, partial_product]() {
      return FactorialWithoutRecursion(i - 1, i * partial_product);
    };
    return ResultOrFunction(fun);
  }
}
