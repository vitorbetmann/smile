/**
 * @file
 * @brief Declarations of internal data types and functions for the
 *        Test module.
 *
 * @author Vitor Betmann
 */

#ifndef SMILE_TEST_H
#define SMILE_TEST_H

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Data Types
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Identifies system functions for failure simulation.
 *
 * Used with tsDisable to specify which function should be
 * forced to fail on a given call count.
 *
 * @author Vitor Betmann
 */
typedef enum
{
    MALLOC,
    CALLOC,
    REALLOC,
    FOPEN,
    MKDIR,
} tsSysFn;

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions -
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Log a "[PASS]" message for a successful test case.
 *
 * @param fnName Name of the test function that passed.
 *
 * @author Vitor Betmann
 */
void tsPass(const char *fnName);

/**
 * @brief Disable a system function for controlled failure simulation.
 *
 * Forces the specified function to fail on the given call count.
 *
 * @param fnName Which function to disable (MALLOC, CALLOC, REALLOC, FOPEN, MKDIR).
 * @param at Call count at which failure should occur.
 *
 * @return true if successfully disabled, false if an invalid function type is given
 *
 * @author Vitor Betmann
 */
bool tsDisable(tsSysFn fnName, unsigned int at);

/**
 * @brief Wrapper around malloc() with optional failure simulation.
 *
 * Use tsDisable(MALLOC, n) to force the nth malloc call to return nullptr.
 *
 * @param size Number of bytes to allocate.
 *
 * @return Pointer to allocated memory, or nullptr if failure is simulated.
 *
 * @author Vitor Betmann
 */
void *tsMalloc(size_t size);

/**
 * @brief Wrapper around calloc() with optional failure simulation.
 *
 * Use tsDisable(CALLOC, n) to force the nth calloc call to return nullptr.
 *
 * @param nitems Number of elements to allocate.
 * @param size Size of each element in bytes.
 *
 * @return Pointer to allocated memory, or nullptr if failure is simulated.
 *
 * @author Vitor Betmann
 */
void *tsCalloc(size_t nitems, size_t size);

/**
 * @brief Wrapper around realloc() with optional failure simulation.
 *
 * Use tsDisable(REALLOC, n) to force the nth realloc call to return nullptr.
 *
 * @param ptr Pointer to a memory block to be reallocated.
 * @param size Number of bytes to allocate.
 *
 * @return Pointer to allocated memory, or nullptr if failure is simulated.
 *
 * @author Vitor Betmann
 */
void *tsRealloc(void *ptr, size_t size);

/**
 * @brief Wrapper around fopen() with optional failure simulation.
 *
 * Use tsDisable(FOPEN, n) to force the nth fopen call to return nullptr.
 *
 * @param path Path of the file to open.
 * @param mode Mode string passed to fopen.
 *
 * @return FILE pointer, or nullptr if failure is simulated.
 *
 * @author Vitor Betmann
 */
FILE *tsFopen(const char *path, const char *mode);

/**
 * @brief Wrapper around mkdir() with optional failure simulation.
 *
 * Use tsDisable(MKDIR, n) to force the nth mkdir call to return -1.
 *
 * @param path Path of the directory to create.
 *
 * @return 0 on success, -1 on failure (real or simulated).
 *
 * @author Vitor Betmann
 */
int tsMkdir(const char *path);

/**
 * @brief Portable wrapper around mkdtemp().
 *
 * Creates a unique temporary directory by replacing the trailing "XXXXXX" in
 * @p tmpl with a unique suffix, then creating the directory. On Windows,
 * uses _mktemp + _mkdir; on POSIX, delegates to mkdtemp.
 *
 * @param tmpl Template string ending in "XXXXXX", modified in-place.
 *
 * @return Pointer to @p tmpl on success, nullptr on failure.
 *
 * @author Vitor Betmann
 */
char *tsMkdtemp(char *tmpl);

/**
 * @brief Reset all failure simulation state to its default (no failures scheduled).
 *
 * Call this at the start of any test that uses tsDisable to guarantee a
 * clean slate, regardless of what a previous test may have left behind.
 *
 * @author Vitor Betmann
 */
void tsReset(void);


#endif
