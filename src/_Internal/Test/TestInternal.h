#ifndef TEST_INTERNAL_H
#define TEST_INTERNAL_H

// --------------------------------------------------
// Includes
// --------------------------------------------------

#include <stdlib.h>
#include <string.h>

// --------------------------------------------------
// Other defines
// --------------------------------------------------

#define TEST_COMP_NAME(name1, name2) strcmp(name1, name2) == 0

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

void TEST_Pass(const char *funcName);

/**
 * @brief Enables or disables simulated malloc behavior for testing.
 *
 * When set to false, TEST_SetCanMalloc will return NULL to simulate allocation
 * failure.
 *
 * @param toggle true to allow malloc, false to simulate failure.
 * @return true if malloc is now allowed, false if it is disabled.
 * @author Vitor Betmann
 */
bool TEST_SetCanMalloc(bool toggle);

bool TEST_SetCanCalloc(bool toggle);

bool TEST_SetCanFatal(bool toggle);

void TEST_SetMallocNum(int num);

void *TEST_Malloc(size_t size);

void TEST_SetCallocNum(int num);

void *TEST_Calloc(size_t num, size_t size);

bool TEST_Fatal(void);

#endif
