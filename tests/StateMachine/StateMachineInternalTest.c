// // --------------------------------------------------
// // Include
// // --------------------------------------------------
//
// #include "StateMachineInternalTest.h"
//
// #include <assert.h>
// #include <stdio.h>
//
// #include "src/StateMachine/StateMachineInternal.h"
// #include "src/_Internal/Test/TestInternal.h"
//
// // --------------------------------------------------
// // Pre-Init - Internal
// // --------------------------------------------------
//
// void Test_SM_Internal_SetCurrState_ReturnsFalsePreInit(void) {
//     assert(!SM_Internal_SetCurrState(&mockState));
//     TEST_Pass("Test_SM_Internal_SetCurrState_ReturnsFalsePreInit");
// }
//
// void Test_SM_Internal_GetCurrState_ReturnsNullPreInit(void) {
//     assert(!SM_Internal_GetCurrState());
//     TEST_Pass("Test_SM_Internal_GetCurrState_ReturnsNullPreInit");
// }
//
// void Test_SM_Internal_GetState_ReturnsNullPreInit(void) {
//     assert(!SM_Internal_GetState("mockState"));
//     TEST_Pass("Test_SM_Internal_GetState_ReturnsNullPreInit");
// }
//
// void Test_SM_Test_GetTracker_ReturnsNullPreInit(void) {
//     assert(!SM_Test_GetTracker());
//     TEST_Pass("Test_SM_Test_GetTracker_ReturnsNullPreInit");
// }
//
// // --------------------------------------------------
// // Post-Init - Internal
// // --------------------------------------------------
//
// void Test_SM_Test_GetTracker_ReturnsTrackerAfterInit(void) {
//     assert(SM_Test_GetTracker());
//     TEST_Pass("Test_SM_Test_GetTracker_ReturnsTrackerAfterInit");
// }
//
// // --------------------------------------------------
// // Pre-Registration - Internal
// // --------------------------------------------------
//
// void Test_SM_Internal_GetCurrState_ReturnsNullIfNoCurrentState(void) {
//     assert(!SM_Internal_GetCurrState());
//     TEST_Pass("Test_SM_Internal_GetCurrState_ReturnsNullIfNoCurrentState");
// }
//
// void Test_SM_Internal_SetCurrState_SetsValidStateCorrectly(void) {
//     SM_Internal_SetCurrState(&mockState);
//     assert(strcmp(SM_Internal_GetCurrState()->name, mockState.name) == 0);
//     TEST_Pass("Test_SM_Internal_SetCurrState_SetsValidStateCorrectly");
// }
//
// void Test_SM_Internal_SetCurrState_SetsNullStateCorrectly(void) {
//     SM_Internal_SetCurrState(NULL);
//     assert(!SM_Internal_GetCurrState());
//     TEST_Pass("Test_SM_Internal_SetCurrState_SetsNULLStateCorrectly");
// }
//
// // --------------------------------------------------
// // Post-Registration - Internal
// // --------------------------------------------------
//
// void Test_SM_Internal_GetState_ReturnsRegisteredStateByName(void) {
//     SM_RegisterState("mockState", mockEnter, NULL, NULL, NULL);
//     assert(strcmp(SM_Internal_GetState("mockState")->name, mockState.name) == 0);
//     TEST_Pass("Test_SM_Internal_GetState_ReturnsRegisteredStateByName");
// }
//
// // --------------------------------------------------
// // Post-Shutdown access - Internal
// // --------------------------------------------------
//
// void Test_SM_Internal_SetCurrState_ReturnsFalseAfterShutdown(void) {
//     assert(!SM_Internal_SetCurrState(&mockState));
//     TEST_Pass("Test_SM_Internal_SetCurrState_ReturnsFalseAfterShutdown");
// }
//
// void Test_SM_Internal_GetCurrState_ReturnsNullAfterShutdown(void) {
//     assert(!SM_Internal_GetCurrState());
//     TEST_Pass("Test_SM_Internal_GetCurrState_ReturnsNullAfterShutdown");
// }
//
// void Test_SM_Internal_GetState_ReturnsNullAfterShutdown(void) {
//     assert(!SM_Internal_GetState("testNoNULL"));
//     TEST_Pass("Test_SM_Internal_GetState_ReturnsNullAfterShutdown");
// }
//
// void Test_SM_Test_GetTracker_ReturnsNullAfterShutdown(void) {
//     assert(!SM_Test_GetTracker());
//     TEST_Pass("Test_SM_Test_GetTracker_ReturnsNullAfterShutdown");
// }
//
// void Test_SM_Update_ReturnsFalseAfterShutdown(void) {
//     assert(!SM_Update(mockDT));
//     TEST_Pass("Test_SM_Update_ReturnsFalseAfterShutdown");
// }
//
// void Test_SM_Draw_ReturnsFalseAfterShutdown(void) {
//     assert(!SM_Draw());
//     TEST_Pass("Test_SM_Draw_ReturnsFalseAfterShutdown");
// }
//
// // --------------------------------------------------
// // Finger's crossed!
// // --------------------------------------------------
//
// int main() {
//     puts("");
//     puts("Testing Pre-Init - Internal");
//     Test_SM_Internal_SetCurrState_ReturnsFalsePreInit();
//     Test_SM_Internal_GetCurrState_ReturnsNullPreInit();
//     Test_SM_Internal_GetState_ReturnsNullPreInit();
//     Test_SM_Test_GetTracker_ReturnsNullPreInit();
//     puts("");
//
//     puts("Testing Pre-Init - Public");
//     Test_SM_IsInitialized_ReturnsFalsePreInit();
//     Test_SM_RegisterState_ReturnsFalsePreInit();
//     Test_SM_ChangeStateTo_ReturnsFalsePreInit();
//     Test_SM_Update_ReturnsFalsePreInit();
//     Test_SM_Draw_ReturnsFalsePreInit();
//     Test_SM_Shutdown_ReturnsFalsePreInit();
//     Test_SM_GetCurrStateName_ReturnsNullPreInit();
//     puts("");
//
//     puts("Testing Init");
//     Test_SM_Init_ReturnsFalseIfMallocFails();
//     Test_SM_Init_ReturnsTrueAndInitializesTracker();
//     Test_SM_Init_ReturnsFalseIfCalledTwice();
//     puts("");
//
//     puts("Testing Post-Init - Internal");
//     Test_SM_Test_GetTracker_ReturnsTrackerAfterInit();
//     puts("");
//
//     puts("Testing Pre-Registration - Internal");
//     Test_SM_Internal_GetCurrState_ReturnsNullIfNoCurrentState();
//     Test_SM_Internal_SetCurrState_SetsValidStateCorrectly();
//     Test_SM_Internal_SetCurrState_SetsNullStateCorrectly();
//     puts("");
//
//     puts("Testing State Registration");
//     Test_SM_RegisterState_ReturnsFalseIfNameIsNULL();
//     Test_SM_RegisterState_ReturnsFalseIfNameIsEmpty();
//     Test_SM_RegisterState_ReturnsTrueIfAllFunctionsPresent();
//     Test_SM_RegisterState_ReturnsFalseIfNameAlreadyExists();
//     Test_SM_RegisterState_ReturnsTrueIfEnterAndUpdateNull();
//     Test_SM_RegisterState_ReturnsTrueIfDrawAndExitNull();
//     Test_SM_RegisterState_ReturnsFalseIfAllFunctionsNull();
//     puts("");
//
//     puts("Testing Post-Registration - Internal");
//     Test_SM_Internal_GetState_ReturnsRegisteredStateByName();
//     puts("");
//
//     puts("Testing Pre-Transitions");
//     Test_SM_GetCurrStateName_ReturnsNullIfCurrentStateIsNull();
//     Test_SM_Update_ReturnsFalseIfCurrentStateIsNull();
//     Test_SM_Draw_ReturnsFalseIfCurrentStateIsNull();
//     puts("");
//
//     puts("Testing Transitions");
//     Test_SM_ChangeStateTo_ReturnsTrueChangingFromNullToValidState();
//     Test_SM_ChangeStateTo_ReturnsFalseIfNameIsNULL();
//     Test_SM_ChangeStateTo_ReturnsFalseIfStateIsUnregistered();
//     Test_SM_ChangeStateTo_ReturnsTrueChangingFromOneValidStateToAnother();
//     Test_SM_ChangeStateTo_CallsExitFunctionOfCurrentState();
//     Test_SM_ChangeStateTo_CallsEnterFunctionOfNewState();
//     Test_SM_ChangeStateTo_CallsExitAndEnterIfChangingToSameState();
//     Test_SM_ChangeStateTo_CallsEnterFunctionWithArgs();
//     puts("");
//
//     puts("Testing Getters");
//     Test_SM_GetCurrStateName_ReturnsCurrentStateName();
//     puts("");
//
//     puts("Testing Checks");
//     Test_SM_IsInitialized_ReturnsTrueAfterInit();
//     Test_SM_IsStateRegistered_ReturnsTrueForValidStateName();
//     Test_SM_IsStateRegistered_ReturnsFalseForInvalidStateName();
//     puts("");
//
//     puts("Testing Update and Draw");
//     Test_SM_Update_CallsValidUpdateFunction();
//     Test_SM_Draw_CallsValidDrawFunction();
//     Test_SM_Update_CallsValidUpdateFunctionEvenIfNullUpdate();
//     Test_SM_Draw_CallsValidDrawFunctionEvenIfNullDraw();
//     puts("");
//
//     puts("Testing Shutdown");
//     Test_SM_Shutdown_CallsExitFunctionOfCurrentState();
//     Test_SM_Shutdown_SkipsExitIfNull();
//     Test_SM_Shutdown_SetsTrackerToNull();
//     Test_SM_Shutdown_SetsStateCountToZero();
//     puts("");
//
//     puts("Testing Post-Shutdown Access - Public");
//     Test_SM_IsInitialized_ReturnsFalseAfterShutdown();
//     Test_SM_GetCurrStateName_ReturnsNullAfterShutdown();
//     Test_SM_RegisterState_ReturnsFalseAfterShutdown();
//     Test_SM_ChangeStateTo_ReturnsFalseAfterShutdown();
//     Test_SM_Shutdown_ReturnsFalseIfCalledMultipleTimesAfterShutdown();
//     puts("");
//
//     puts("Testing Post-Shutdown Access - Internal");
//     Test_SM_Internal_SetCurrState_ReturnsFalseAfterShutdown();
//     Test_SM_Internal_GetCurrState_ReturnsNullAfterShutdown();
//     Test_SM_Internal_GetState_ReturnsNullAfterShutdown();
//     Test_SM_Test_GetTracker_ReturnsNullAfterShutdown();
//     Test_SM_Update_ReturnsFalseAfterShutdown();
//     Test_SM_Draw_ReturnsFalseAfterShutdown();
//     puts("");
//
//     puts("Testing Stress Tests");
//     Test_SM_RegisteringMultipleStatesCausesNoSkips();
//     Test_SM_ChangingStatesOftenCausesNoSkips();
//     Test_SM_Shutdown_FreeingMultipleStatesCausesNoSkips();
//     puts("");
//
//     puts("\nTIME TO SMILE! :)\n\tAll Tests Passed!");
//     return 0;
// }
