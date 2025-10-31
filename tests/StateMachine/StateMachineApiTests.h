#ifndef SMILE_STATE_MACHINE_API_TESTS_H
#define SMILE_STATE_MACHINE_API_TESTS_H

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Other Defines
// -----------------------------------------------------------------------------

#define STRESS_ITERATIONS 1000
#define IDEMPOTENT_ITERATIONS 3


// -----------------------------------------------------------------------------
// Data types
// -----------------------------------------------------------------------------

typedef struct {
    bool enterCalled;
    bool exitCalled;
} MockData;

typedef void (*smTestExitFn)(MockData *args);

typedef struct {
    bool flag;
} MockStateArgs;


// -----------------------------------------------------------------------------
// Prototypes
// -----------------------------------------------------------------------------

extern smTestExitFn smTestExit;
extern MockData *smMockData;


#endif // #ifndef SMILE_STATE_MACHINE_API_TESTS_H
