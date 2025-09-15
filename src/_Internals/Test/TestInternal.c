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
static bool canFatal = false;

// --------------------------------------------------
// Functions
// --------------------------------------------------

bool TEST_SetCanMalloc(bool toggle) {
  if (canMalloc == toggle) {
    return false;
  }

  canMalloc = toggle;
  return true;
}

bool TEST_SetCanCalloc(bool toggle) {

  if (canCalloc == toggle) {
    return false;
  }

  canCalloc = toggle;
  return true;
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

bool TEST_SetCanFatal(bool toggle) {
  if (canFatal == toggle) {
    return false;
  }

  canFatal = toggle;
  return true;
}

bool TEST_Fatal(void) { return canFatal; }