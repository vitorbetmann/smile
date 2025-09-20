// --------------------------------------------------
// Includes
// --------------------------------------------------

#include "src/_Internal/Test/TestInternal.h"
#include <stdio.h>

// --------------------------------------------------
// Defines
// --------------------------------------------------

// --------------------------------------------------
// Variables
// --------------------------------------------------

static bool canMalloc = true;
static bool canCalloc = true;
static bool canFatal = false;
static int mallocNum;
static int callocNum;

// --------------------------------------------------
// Functions
// --------------------------------------------------

void TEST_Pass(const char *funcName) { printf("\t[PASS] %s\n", funcName); }

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

void TEST_SetMallocNum(const int num) {
  mallocNum = num;
}

void *TEST_Malloc(size_t size) {
  mallocNum--;
  if (mallocNum == 0 && !canMalloc) {
    return NULL;
  }

  return malloc(size);
}

void TEST_SetCallocNum(const int num) {
  callocNum = num;
}

void *TEST_Calloc(const size_t num, const size_t size) {
  callocNum--;
  if (callocNum == 0 && !canCalloc) {
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
