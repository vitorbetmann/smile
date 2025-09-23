/**
 * @file TestInternal.h
 * @brief Instrumented memory allocation and fatal hooks for SMILE.
 *
 * Provides wrappers around malloc, calloc, realloc, and fatal conditions to
 * support controlled allocation behavior, error handling, and logging.
 */

#ifndef TEST_INTERNAL_H
#define TEST_INTERNAL_H

#include <stdlib.h>

/**
 * @brief Identifies allocation functions for failure simulation.
 *
 * Used with TEST_Disable to specify which type of allocation
 * should be forced to fail.
 */
typedef enum {
    MALLOC,
    CALLOC,
    REALLOC,
} MemAllocFunc;

/**
 * @brief Log a "[PASS]" message for a successful test case.
 *
 * @param funcName Name of the test function that passed
 */
void TEST_Pass(const char *funcName);

/**
 * @brief Disable a memory allocation function for controlled failure.
 *
 * Forces the specified allocation function to fail on the given call count.
 * After the failure occurs, the function resumes normal behavior.
 *
 * @param funcName Which allocation function to disable (MALLOC, CALLOC, REALLOC)
 * @param at Call count at which failure should occur
 * @return true if successfully disabled, false if an invalid function type is given
 */
bool TEST_Disable(MemAllocFunc funcName, int at);

/**
 * @brief Wrapper around malloc() with optional failure simulation.
 *
 * Use TEST_Disable(MALLOC, n) to force the nth malloc call to return NULL.
 *
 * @param size Number of bytes to allocate
 * @return Pointer to allocated memory, or NULL if failure is simulated
 */
void *TEST_Malloc(size_t size);

/**
 * @brief Wrapper around calloc() with optional failure simulation.
 *
 * Use TEST_Disable(CALLOC, n) to force the nth calloc call to return NULL.
 *
 * @param nitems Number of elements to allocate
 * @param size Size of each element in bytes
 * @return Pointer to allocated memory, or NULL if failure is simulated
 */
void *TEST_Calloc(size_t nitems, size_t size);

/**
 * @brief Wrapper around realloc() with optional failure simulation.
 *
 * @param ptr Pointer to a memory block to be reallocated
 * @param size Number of bytes to allocate
 * @return Pointer to allocated memory, or NULL if failure is simulated
 */
void *TEST_Realloc(void *ptr, size_t size);

/**
 * @brief Check whether a fatal condition is being simulated.
 *
 * Can be used in both tests and production code to handle special
 * cases where fatal behavior is expected.
 *
 * @return true if a fatal condition is active, false otherwise
 */
bool TEST_Fatal(void);

#endif // TEST_INTERNAL_H
