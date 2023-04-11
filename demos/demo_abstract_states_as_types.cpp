#include "../abstract_states_as_types.h"

int main() {
  ProcessStateInVariable process_state_in_variable;
  process_state_in_variable.StateTransition(ProcessState::Ready);
  process_state_in_variable.StateTransition(ProcessState::Running);
  process_state_in_variable.StateTransition(ProcessState::Waiting);
  process_state_in_variable.StateTransition(ProcessState::Running);
  process_state_in_variable.StateTransition(ProcessState::Terminated);

  ProcessStateInType<ProcessState::Created> processStateInType;
  auto ready_process = TransitionToReady(process_state_in_variable);
  auto running_process = TransitionToRunning(ready_process);
  auto waiting_process = TransitionToWaiting(running_process);
  ready_process = TransitionToReady(waiting_process);
  running_process = TransitionToRunning(ready_process);
  auto terminated_process = TransitionToTerminated(running_process);
  return 0;
}
