// --------------------------------------------------
// Includes
// --------------------------------------------------

#include "src/_Internals/Test/TestInternal.h"

// --------------------------------------------------
// Defines
// --------------------------------------------------

// --------------------------------------------------
// Variables
// --------------------------------------------------

static bool canMalloc = true;
static bool canCalloc = true;

// --------------------------------------------------
// Functions
// --------------------------------------------------

bool TEST_SetCanMalloc(bool toggle) {
  canMalloc = toggle;
  return toggle;
}

bool TEST_SetCanCalloc(bool toggle) {
  canCalloc = toggle;
  return toggle;
}

void *TEST_Malloc(size_t size) {
  if (!canMalloc) {
    return NULL;
  }
  return malloc(size);
}

void *TEST_Calloc(size_t num, size_t size) {
  if (!canCalloc) {
    return NULL;
  }
  return calloc(num, size);
}