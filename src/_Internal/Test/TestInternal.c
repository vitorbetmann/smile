/**
* @file TestInternal.c
 * @brief Implementation of test utilities for simulating allocation failures.
 *
 * Internal use only. Provides definitions for the functions declared in
 * TestInternal.h.
 */

#include "src/_Internal/Test/TestInternal.h"
#include <stdio.h>

// --------------------------------------------------
// Internal state
// --------------------------------------------------

static bool canMalloc = true;
static bool canCalloc = true;
static bool canRealloc = true;
static bool canFatal = false;

static int mallocNum;
static int callocNum;
static int reallocNum;

// --------------------------------------------------
// Functions
// --------------------------------------------------

void TEST_Pass(const char *funcName) {
  printf("\t[PASS] %s\n", funcName);
}

bool TEST_Disable(const MemAllocFunc funcName, const int at) {
  switch (funcName) {
    case MALLOC:
      canMalloc = false;
      mallocNum = at;
      return true;
    case CALLOC:
      canCalloc = false;
      callocNum = at;
      return true;
    case REALLOC:
      canRealloc = false;
      reallocNum = at;
      return true;
    default:
      return false;
  }
}

void *TEST_Malloc(const size_t size) {
  mallocNum--;
  if (!canMalloc && mallocNum == 0) {
    canMalloc = true;
    return NULL;
  }
  return malloc(size);
}

void *TEST_Calloc(const size_t nitems, const size_t size) {
  callocNum--;
  if (!canCalloc && callocNum == 0) {
    canCalloc = true;
    return NULL;
  }
  return calloc(nitems, size);
}

void *TEST_Realloc(void *ptr, const size_t size) {
  reallocNum--;
  if (!canRealloc && reallocNum == 0) {
    canRealloc = true;
    return NULL;
  }
  return realloc(ptr, size);
}

bool TEST_Fatal(void) {
  return canFatal;
}
