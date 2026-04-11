/**
 * @file
 * @brief Implementation of the TestInternal module.
 *
 * @see TestInternal.h
 *
 * @author Vitor Betmann
 */

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Include
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include "TestInternal.h"


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// TestInternal state
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

static bool canMalloc = true;
static bool canCalloc = true;
static bool canRealloc = true;
static bool canFopen = true;
static bool canMkdir = true;

static unsigned int mallocNum;
static unsigned int callocNum;
static unsigned int reallocNum;
static unsigned int fopenNum;
static unsigned int mkdirNum;


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void tsInternalPass(const char *fnName)
{
    printf("\t[PASS] %s\n", fnName);
}

bool tsInternalDisable(InternalSysFn fnName, unsigned int at)
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
    case FOPEN:
        canFopen = false;
        fopenNum = at;
        return true;
    case MKDIR:
        canMkdir = false;
        mkdirNum = at;
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

FILE *tsInternalFopen(const char *path, const char *mode)
{
    fopenNum--;
    if (!canFopen && fopenNum == 0)
    {
        canFopen = true;
        return nullptr;
    }
    return fopen(path, mode);
}

int tsInternalMkdir(const char *path)
{
    mkdirNum--;
    if (!canMkdir && mkdirNum == 0)
    {
        canMkdir = true;
        return -1;
    }
#ifdef _WIN32
    return _mkdir(path);
#else
    return mkdir(path, 0755);
#endif
}
