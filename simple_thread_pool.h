#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>

class SimpleThreadPool {
public:
  explicit SimpleThreadPool(int threads_num)
      : threads_num(threads_num), state(State::Stop) {}
  SimpleThreadPool(const SimpleThreadPool &) = delete;
  SimpleThreadPool(const SimpleThreadPool &&) = delete;
  SimpleThreadPool operator=(const SimpleThreadPool &) = delete;
  SimpleThreadPool operator=(const SimpleThreadPool &&) = delete;
  ~SimpleThreadPool();
  void Start();
  void Stop(bool wait_for_all_finish);
  template <typename F, typename... Args>
  void PushTask(F &&fun, Args &&...args);
  template <class F, typename... Args,
            typename R = std::invoke_result_t<F, Args...>>
  std::future<R> PushTaskWithRetrunValue(F &&fun, Args &&...args);
  void WaitTasksFinish();

private:
  enum class State {
    Runing,
    WatingForFinish,
    Stop,
  };
  int threads_num;
  std::atomic<State> state;
  std::vector<std::thread> threads;
  std::mutex mut;
  std::condition_variable task_avaliable_cond;
  std::condition_variable task_finish_cond;
  std::queue<std::function<void()>> tasks_queue;

  void worker();
};

SimpleThreadPool::~SimpleThreadPool() { Stop(false); }

void SimpleThreadPool::Start() {
  state = State::Runing;
  for (int i = 0; i < threads_num; ++i) {
    threads.emplace_back(&SimpleThreadPool::worker, this);
  }
}

void SimpleThreadPool::WaitTasksFinish() {
  std::for_each(threads.begin(), threads.end(), [](auto &th) {
    if (th.joinable()) {
      th.join();
    }
  });
}

void SimpleThreadPool::Stop(bool wait_for_all_finish) {
  if (wait_for_all_finish) {
    state = State::WatingForFinish;
    std::unique_lock<std::mutex> lock(mut);
    task_finish_cond.wait(lock, [this] { return tasks_queue.empty(); });
  }
  state = State::Stop;
  task_avaliable_cond.notify_all();
  WaitTasksFinish();
  std::queue<std::function<void()>>().swap(tasks_queue);
}

template <typename F, typename... Args>
void SimpleThreadPool::PushTask(F &&fun, Args &&...args) {
  if (state != State::Runing) {
    return;
  }
  std::function<void()> task =
      std::bind(std::forward<F>(fun), std::forward<Args>(args)...);
  {
    std::unique_lock<std::mutex> lock(mut);
    tasks_queue.push(std::move(task));
  }
  task_avaliable_cond.notify_one();
}

void SimpleThreadPool::worker() {
  while (state != State::Stop) {
    std::function<void()> task;
    {
      std::unique_lock<std::mutex> lock(mut);
      task_avaliable_cond.wait(lock, [this] {
        return !tasks_queue.empty() || state == State::Stop;
      });
      if (state == State::Stop) {
        return;
      }
      task = std::move(tasks_queue.front());
      tasks_queue.pop();
    }
    task();
    task_finish_cond.notify_one();
  }
}
template <class F, typename... Args, typename R>
std::future<R> SimpleThreadPool::PushTaskWithRetrunValue(F &&fun,
                                                         Args &&...args) {
  std::function<R()> task_with_ret =
      std::bind(std::forward<F>(fun), std::forward<Args>(args)...);
  std::shared_ptr<std::promise<R>> task_promise =
      std::make_shared<std::promise<R>>();
  ;
  PushTask([task_with_ret, task_promise] {
    task_promise->set_value(std::invoke(task_with_ret));
  });
  return task_promise->get_future();
}