/**
 * @file
 * @brief Internal declarations of data structures and functions for the
 *        TestInternal module.
 *
 * Provides wrappers around malloc, calloc, realloc, and fatal conditions to
 * support controlled allocation behavior, error handling, and logging.
 *
 * @author Vitor Betmann
 */

#ifndef SMILE_TEST_INTERNAL_H
#define SMILE_TEST_INTERNAL_H

// —————————————————————————————————————————————————————————————————————————————
// Data Types
// —————————————————————————————————————————————————————————————————————————————

/**
 * @brief Identifies allocation functions for failure simulation.
 *
 * Used with tsInternalDisable to specify which type of allocation
 * should be forced to fail.
 *
 * @author Vitor Betmann
 */
typedef enum {
    MALLOC,
    CALLOC,
    REALLOC,
} InternalMemAllocFn;

// —————————————————————————————————————————————————————————————————————————————
// Functions - Internal
// —————————————————————————————————————————————————————————————————————————————

/**
 * @brief LogInternal a "[PASS]" message for a successful test case.
 *
 * @param fnName Name of the test function that passed.
 *
 * @author Vitor Betmann
 */
void tsInternalPass(const char *fnName);

/**
 * @brief Disable a memory allocation function for controlled failure.
 *
 * Forces the specified allocation function to fail on the given call count.
 *
 * @param fnName Which allocation function to disable (MALLOC, CALLOC, REALLOC).
 * @param at Call count at which failure should occur.
 *
 * @return true if successfully disabled, false if an invalid function type is given
 *
 * @author Vitor Betmann
 */
bool tsInternalDisable(InternalMemAllocFn fnName, unsigned int at);

/**
 * @brief Wrapper around malloc() with optional failure simulation.
 *
 * Use tsInternalDisable(MALLOC, n) to force the nth malloc call to return nullptr.
 *
 * @param size Number of bytes to allocate.
 *
 * @return Pointer to allocated memory, or nullptr if failure is simulated.
 *
 * @author Vitor Betmann
 */
void *tsInternalMalloc(size_t size);

/**
 * @brief Wrapper around calloc() with optional failure simulation.
 *
 * Use tsInternalDisable(CALLOC, n) to force the nth calloc call to return nullptr.
 *
 * @param nitems Number of elements to allocate.
 * @param size Size of each element in bytes.
 *
 * @return Pointer to allocated memory, or nullptr if failure is simulated.
 *
 * @author Vitor Betmann
 */
void *tsInternalCalloc(size_t nitems, size_t size);

/**
 * @brief Wrapper around realloc() with optional failure simulation.
 *
 * @param ptr Pointer to a memory block to be reallocated.
 * @param size Number of bytes to allocate.
 *
 * @return Pointer to allocated memory, or nullptr if failure is simulated.
 *
 * @author Vitor Betmann
 */
void *tsInternalRealloc(void *ptr, size_t size);


#endif // #ifndef SMILE_TEST_INTERNAL_H