#ifndef SMILE_STATE_MACHINE_API_TESTS_H
#define SMILE_STATE_MACHINE_API_TESTS_H

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Other Defines
// -----------------------------------------------------------------------------

#define NANO_SEC_PER_SEC 1000000000

#define STRESS_ITERATIONS 1000
#define FRAME_TIME_ITERATIONS 300
#define IDEMPOTENT_ITERATIONS 3

#define SM_GET_DT_FIRST_CALL_TIME_NS 16667000
#define DT_TOLERANCE 1e-6f
#define EXPECTED_DT  0.016667f


// -----------------------------------------------------------------------------
// Data types
// -----------------------------------------------------------------------------

typedef struct {
    int enterCount;
    int exitCount;
} MockData;

typedef struct {
    bool flag;
} MockArgs;

typedef void (*smTestEnterFn)(MockData *data);

typedef void (*smTestEnterWithArgsFn)(MockData *data, MockArgs *args);

typedef void (*smTestExitFn)(MockData *data);


// -----------------------------------------------------------------------------
// Prototypes
// -----------------------------------------------------------------------------

extern smTestEnterFn smTestEnter;
extern smTestEnterWithArgsFn smTestEnterWithArgs;
extern smTestExitFn smTestExit;
extern MockData *smMockData;
extern MockArgs *smMockArgs;
extern struct timespec smMockCurrTime;

#endif // #ifndef SMILE_STATE_MACHINE_API_TESTS_H
