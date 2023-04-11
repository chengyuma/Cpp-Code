#include <cstdlib>
#include <type_traits>

// Two examples of process state switching.
// The first one uses a variable to store process state.
// The second abstract states as types.
// And in Linux, the different states of a process are represented by the PCB in
// different linked lists.

enum class ProcessState {
  Created,
  Ready,
  Running,
  Waiting,
  Terminated,
};

class ProcessBase {
public:
  ProcessBase() : PID(GenPID()) {}
  ProcessBase(const ProcessBase &) = default;
  int GetPID() { return PID; }

private:
  int GenPID() { return rand(); }

  int PID;
};

// First one uses a variable to store the state
// Call StateTransition() will return whether the transition successful
// The check of the state transitions is legal at runtime
class ProcessStateInVariable : public ProcessBase {
public:
  ProcessStateInVariable() : ProcessBase(), state(ProcessState::Created) {}
  bool StateTransition(ProcessState state_expect);

private:
  ProcessState state;
};

bool ProcessStateInVariable::StateTransition(ProcessState state_expect) {
  if (this->state == ProcessState::Created) {
    if (state_expect == ProcessState::Ready) {
      this->state = state_expect;
      return true;
    } else {
      return 0;
    }
  } else if (this->state == ProcessState::Ready) {
    if (state_expect == ProcessState::Running) {
      this->state = state_expect;
      return true;
    } else {
      return 0;
    }
  } else if (this->state == ProcessState::Running) {
    if (state_expect == ProcessState::Ready ||
        state_expect == ProcessState::Waiting ||
        state_expect == ProcessState::Terminated) {
      this->state = state_expect;
      return true;
    } else {
      return 0;
    }
  } else if (this->state == ProcessState::Waiting) {
    if (state_expect == ProcessState::Ready) {
      this->state = state_expect;
      return true;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

// The second abstract states as types
// The check of the state transitions is legal during the compilation
template <ProcessState state> class ProcessStateInType : public ProcessBase {
public:
  ProcessStateInType() : ProcessBase() {}
  explicit ProcessStateInType(const ProcessBase &process_base)
      : ProcessBase(process_base) {}
};

template <typename T> struct can_to_ready {
  enum { value = 0 };
};
template <> struct can_to_ready<ProcessStateInType<ProcessState::Created>> {
  enum { value = 1 };
};
template <> struct can_to_ready<ProcessStateInType<ProcessState::Running>> {
  enum { value = 1 };
};
template <> struct can_to_ready<ProcessStateInType<ProcessState::Waiting>> {
  enum { value = 1 };
};

template <typename T> struct can_to_running {
  enum { value = 0 };
};
template <> struct can_to_running<ProcessStateInType<ProcessState::Ready>> {
  enum { value = 1 };
};

template <typename T> struct can_to_waiting {
  enum { value = 0 };
};
template <> struct can_to_waiting<ProcessStateInType<ProcessState::Running>> {
  enum { value = 1 };
};

template <typename T> struct can_to_terminated {
  enum { value = 0 };
};
template <>
struct can_to_waiting<ProcessStateInType<ProcessState::Terminated>> {
  enum { value = 1 };
};

template <typename T, typename T2 = std::enable_if<can_to_ready<T>::value>>
ProcessStateInType<ProcessState::Ready> TransitionToReady(T process) {
  return ProcessStateInType<ProcessState::Ready>(process);
}

template <typename T, typename T2 = std::enable_if<can_to_running<T>::value>>
ProcessStateInType<ProcessState::Running> TransitionToRunning(T process) {
  return ProcessStateInType<ProcessState::Running>(process);
}

template <typename T, typename T2 = std::enable_if<can_to_waiting<T>::value>>
ProcessStateInType<ProcessState::Waiting> TransitionToWaiting(T process) {
  return ProcessStateInType<ProcessState::Waiting>(process);
}

template <typename T, typename T2 = std::enable_if<can_to_terminated<T>::value>>
ProcessStateInType<ProcessState::Terminated> TransitionToTerminated(T process) {
  return ProcessStateInType<ProcessState::Terminated>(process);
}
