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
#include <io.h>
#else
#include <sys/stat.h>
#include <unistd.h>
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
    if (at == 0)
        return false;
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

void tsInternalReset(void)
{
    canMalloc = true;
    canCalloc = true;
    canRealloc = true;
    canFopen = true;
    canMkdir = true;

    mallocNum = 0;
    callocNum = 0;
    reallocNum = 0;
    fopenNum = 0;
    mkdirNum = 0;
}

char *tsInternalMkdtemp(char *tmpl)
{
#ifdef _WIN32
    if (_mktemp(tmpl) == nullptr)
        return nullptr;
    if (_mkdir(tmpl) != 0)
        return nullptr;
    return tmpl;
#else
    return mkdtemp(tmpl);
#endif
}
