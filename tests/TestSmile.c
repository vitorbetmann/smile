#include "../include/Smile.h"
#include <assert.h>
#include <stdio.h>

void Test_SM_Init_SetsStateToNull(void) {
  SM_Init();
  assert(SM_GetCurrState() == NULL);
  puts("\t[PASS] sm_init sets curr_state to NULL");
}

State dummy = {0};
void Test_SM_ChangeStateWorks(void) {
  SM_ChangeState(&dummy, NULL);
  assert(SM_GetCurrState() == &dummy);
  puts("\t[PASS] sm_change_state works correctly");
}

void Test_SM_Shutdown_SetsStateToNull(void) {
  SM_Shutdown();
  assert(SM_GetCurrState() == NULL);
  puts("\t[PASS] sm_shutdown sets curr_state to NULL");
}

int main() {
  puts("");
  puts("Testing Initialization");
  Test_SM_Init_SetsStateToNull();
  puts("Testing Transition");
  Test_SM_ChangeStateWorks();
  puts("Testing Shutdown");
  Test_SM_Shutdown_SetsStateToNull();
  puts("");
  puts("ALL TESTS PASSED!!");
  return 0;
}