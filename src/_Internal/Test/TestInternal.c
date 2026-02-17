/**
 * @file
 * @brief Implementation of the TestInternal module.
 *
 * @see TestInternal.h
 *
 * @bug No known bugs.
 *
 * @author Vitor Betmann
 * @date 2026-02-17
 * @version 1.1.0
 */

// —————————————————————————————————————————————————————————————————————————————
// Include
// —————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <stdlib.h>

#include "TestInternal.h"


// —————————————————————————————————————————————————————————————————————————————
// TestInternal state
// —————————————————————————————————————————————————————————————————————————————

static bool canMalloc = true;
static bool canCalloc = true;
static bool canRealloc = true;

static unsigned int mallocNum;
static unsigned int callocNum;
static unsigned int reallocNum;


// —————————————————————————————————————————————————————————————————————————————
// Functions
// —————————————————————————————————————————————————————————————————————————————

void tsInternalPass(const char *fnName)
{
    printf("\t[PASS] %s\n", fnName);
}

bool tsInternalDisable(InternalMemAllocFn fnName, unsigned int at)
{
    switch (fnName)
    {
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

void *tsInternalMalloc(const size_t size)
{
    mallocNum--;
    if (!canMalloc && mallocNum == 0)
    {
        canMalloc = true;
        return nullptr;
    }
    return malloc(size);
}

void *tsInternalCalloc(const size_t nitems, const size_t size)
{
    callocNum--;
    if (!canCalloc && callocNum == 0)
    {
        canCalloc = true;
        return nullptr;
    }
    return calloc(nitems, size);
}

void *tsInternalRealloc(void *ptr, const size_t size)
{
    reallocNum--;
    if (!canRealloc && reallocNum == 0)
    {
        canRealloc = true;
        return nullptr;
    }
    return realloc(ptr, size);
}
