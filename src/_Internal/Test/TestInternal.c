/**
* @file TestInternal.c
 * @brief Implementation of test utilities for simulating allocation failures.
 *
 * Provides definitions for the functions declared in TestInternal.h.
 */

#include <stdio.h>
#include <stdlib.h>

#include "TestInternal.h"

// --------------------------------------------------
// TestInternal state
// --------------------------------------------------

static bool canMalloc = true;
static bool canCalloc = true;
static bool canRealloc = true;
// static bool canFatal = false;

static unsigned int mallocNum;
static unsigned int callocNum;
static unsigned int reallocNum;

// --------------------------------------------------
// Functions
// --------------------------------------------------

void tsInternalPass(const char *fnName) {
    printf("\t[PASS] %s\n", fnName);
}

bool tsInternalDisable(InternalMemAllocFn fnName, unsigned int at) {
    switch (fnName) {
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

void *tsInternalMalloc(const size_t size) {
    mallocNum--;
    if (!canMalloc && mallocNum == 0) {
        canMalloc = true;
        return nullptr;
    }
    return malloc(size);
}

void *tsInternalCalloc(const size_t nitems, const size_t size) {
    callocNum--;
    if (!canCalloc && callocNum == 0) {
        canCalloc = true;
        return nullptr;
    }
    return calloc(nitems, size);
}

void *tsInternalRealloc(void *ptr, const size_t size) {
    reallocNum--;
    if (!canRealloc && reallocNum == 0) {
        canRealloc = true;
        return nullptr;
    }
    return realloc(ptr, size);
}
