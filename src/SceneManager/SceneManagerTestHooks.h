#pragma once

// Data Types ——————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Holds invocation counters shared across all test hook callbacks.
 */
typedef struct
{
    int enterCount; /**< Incremented each time a scene enter callback fires. */
    int exitCount;  /**< Incremented each time a scene exit callback fires.  */
} MockData;

/**
 * @brief Carries arbitrary test arguments into enter callbacks.
 */
typedef struct
{
    bool flag; /**< Arbitrary boolean carried into the enter callback. */
} MockArgs;

/** @brief Function pointer type for intercepting enter callbacks that take no user arguments. */
typedef void (*smTestEnterFn)(MockData *data);

/** @brief Function pointer type for intercepting enter callbacks that receive a MockArgs *. */
typedef void (*smTestEnterWithArgsFn)(MockData *data, MockArgs *args);

/** @brief Function pointer type for intercepting exit callbacks. */
typedef void (*smTestExitFn)(MockData *data);

// Variables  ——————————————————————————————————————————————————————————————————————————————————————

/** @brief Interceptor for enter callbacks that take no args. */
extern smTestEnterFn smTestEnter;

/** @brief Interceptor for enter callbacks that receive a MockArgs *. */
extern smTestEnterWithArgsFn smTestEnterWithArgs;

/** @brief Interceptor for exit callbacks. */
extern smTestExitFn smTestExit;

/** @brief Arguments pointer forwarded to smTestEnterWithArgs. */
extern MockArgs *smMockArgs;

/** @brief Shared counter struct threaded through all three interceptors. */
extern MockData *smMockData;

/** @brief Overrides the real clock in smGetDt() to simulate elapsed time. */
extern struct timespec smMockCurrTime;

/** @brief When true, forces smGetDt() to return RES_CLOCK_GETTIME_FAIL. */
extern bool smMockClockGettimeFails;