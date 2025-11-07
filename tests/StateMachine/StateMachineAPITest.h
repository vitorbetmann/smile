/**
 * @file
 * @brief Declarations of data types and variables for the StateMachine module
 *        and its test suite.
 *
 * @see StateMachine.c
 * @see StateMachineAPITests.c
 *
 * @author Vitor Betmann
 * @date 2025-11-04
 * @version 1.0.0
 */

#ifndef SMILE_STATE_MACHINE_API_TESTS_H
#define SMILE_STATE_MACHINE_API_TESTS_H

// -----------------------------------------------------------------------------
// Defines
// -----------------------------------------------------------------------------

#define NANOSEC_PER_SEC 1000000000

#define STRESS_ITERATIONS 1000
#define FRAME_TIME_ITERATIONS 300
#define IDEMPOTENT_ITERATIONS 3

#define SM_GET_DT_FIRST_CALL_TIME_NS 16667000
#define DT_TOLERANCE 1e-6f
#define EXPECTED_DT  0.016667f


// -----------------------------------------------------------------------------
// Data Types
// -----------------------------------------------------------------------------

/**
 * @brief Stores counters and mock state data used during test execution.
 */
typedef struct {
    int enterCount;
    int exitCount;
} MockData;

/**
 * @brief Represents a simple mock argument structure passed to state functions.
 */
typedef struct {
    bool flag;
} MockArgs;

/**
 * @brief Function pointer type for testing state entry callbacks without
 *        arguments.
 *
 * @param data Pointer to  MockData used to track invocation counts.
 *
 * @author Vitor Betmann
 */
typedef void (*smTestEnterFn)(MockData *data);

/**
 * @brief Function pointer type for testing state entry callbacks that receive
 *        arguments.
 *
 * @param data Pointer to MockData used to track invocation counts.
 * @param args Pointer to  MockArgs containing the mock arguments.
 *
 * @author Vitor Betmann
 */
typedef void (*smTestEnterWithArgsFn)(MockData *data, MockArgs *args);

/**
 * @brief Function pointer type for testing state exit callbacks.
 *
 * @param data Pointer to  MockData used to track invocation counts.
 *
 * @author Vitor Betmann
 */
typedef void (*smTestExitFn)(MockData *data);


// -----------------------------------------------------------------------------
// Variables
// -----------------------------------------------------------------------------

extern smTestEnterFn smTestEnter;
extern smTestEnterWithArgsFn smTestEnterWithArgs;
extern smTestExitFn smTestExit;
extern MockData *smMockData;
extern MockArgs *smMockArgs;
extern struct timespec smMockCurrTime;


#endif // #ifndef SMILE_STATE_MACHINE_API_TESTS_H
