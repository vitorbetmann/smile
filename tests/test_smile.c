#include "../include/smile.h"
#include <assert.h>
#include <stdio.h>

void test_sm_init_sets_state_to_null(void) {
  sm_init();
  assert(sm_get_curr_state() == NULL);
  puts("\t[PASS] sm_init sets curr_state to NULL");
}

State dummy = {0};
void test_sm_change_state_works(void) {
  sm_change_state(&dummy, NULL);
  assert(sm_get_curr_state() == &dummy);
  puts("\t[PASS] sm_change_state works correctly");
}

void test_sm_shutdown_sets_state_to_null(void) {
  sm_shutdown();
  assert(sm_get_curr_state() == NULL);
  puts("\t[PASS] sm_shutdown sets curr_state to NULL");
}

int main() {
  puts("");
  puts("Testing Initialization");
  test_sm_init_sets_state_to_null();
  puts("Testing Transition");
  test_sm_change_state_works();
  puts("Testing Shutdown");
  test_sm_shutdown_sets_state_to_null();
  puts("");
  puts("ALL TESTS PASSED!!");
  return 0;
}