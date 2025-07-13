#include "../include/StateMachine.h"
#include "StateMachineTestAccess.h"
#include <assert.h>
#include <stdio.h>

void Test_SM_Init_InitializesTracker(void) {
  SM_Init();
  assert(SM_Test_GetTracker());
  puts("\t[PASS] SM_INIT Initializes Tracker");
}

// State dummy = {0};
// void Test_SM_ChangeStateToWorks(void) {
//   SM_ChangeStateTo(&dummy, NULL);
//   assert(SM_GetCurrState() == &dummy);
//   puts("\t[PASS] sm_change_state works correctly");
// }

// void Test_SM_Shutdown_SetsStateToNull(void) {
//   SM_Shutdown();
//   assert(SM_GetCurrState() == NULL);
//   puts("\t[PASS] sm_shutdown sets curr_state to NULL");
// }

int main() {
  puts("");
  puts("Testing Initialization");
  Test_SM_Init_InitializesTracker();

  puts("Testing State Registration");
  puts("Testing Transition");
  puts("Testing Update and Draw");

  puts("Testing Shutdown");
  puts("Testing after Shutdown access");

  puts("");
  puts("ALL TESTS PASSED!!");
  return 0;
}