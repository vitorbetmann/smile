/**
 * @file
 * @brief Implementation of the CommonInternal module.
 *
 * @see CommonInternal.h
 * @see CommonInternalMessages.h
 *
 * @author Vitor Betmann
 */


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#ifndef _WIN32
#define _XOPEN_SOURCE 700
#endif

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif
#include <sys/stat.h>

#include "CommonInternal.h"
#include "CommonInternalMessages.h"

#include "LogInternal.h"
#include "TestInternal.h"


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

// Start Related

bool cmInternalIsRunning(cmIsRunningFn cmIsRunning, const char *module,
                         const char *fnName)
{
    if (!cmIsRunning())
    {
        lgInternalLog(ERROR, module, CAUSE_NOT_RUNNING, fnName, CONSEQ_ABORTED);
        return false;
    }

    return true;
}

// Filesystem

bool cmInternalDirExists(const char *path)
{
#ifdef _WIN32
    struct _stat sb;
    return _stat(path, &sb) == 0 && sb.st_mode & _S_IFDIR;
#else
    struct stat sb;
    return (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode));
#endif
}


int cmInternalValidatePath(const char *path)
{
    if (!path)
    {
        return CM_RESULT_NULL_ARG;
    }
    if (!path[0])
    {
        return CM_RESULT_EMPTY_ARG;
    }

#ifdef _WIN32
    if (path[0] == '/' || path[0] == '\\' || (isalpha((unsigned char)path[0]) && path[1] == ':'))
    {
        return CM_RESULT_INVALID_PATH;
    }
#else
    if (path[0] == '/')
    {
        return CM_RESULT_INVALID_PATH;
    }
#endif

    size_t len = strnlen(path, CM_PATH_MAX);
    if (len >= CM_PATH_MAX)
    {
        return CM_RESULT_INVALID_PATH;
    }

    // Reject any bare ".." segment to prevent escaping cwd
    char tmp[CM_PATH_MAX];
    memcpy(tmp, path, len + 1);
    char *seg = tmp;
    for (char *p = tmp; ; p++)
    {
#ifdef _WIN32
        if (*p == '/' || *p == '\\' || *p == '\0')
#else
        if (*p == '/' || *p == '\0')
#endif
        {
            char saved = *p;
            *p = '\0';
            if (strcmp(seg, "..") == 0)
            {
                return CM_RESULT_INVALID_PATH;
            }
            *p = saved;
            if (saved == '\0')
            {
                break;
            }
            seg = p + 1;
        }
    }

    return CM_RESULT_OK;
}


int cmInternalCreateDir(const char *path)
{
    int result = cmInternalValidatePath(path);
    if (result != CM_RESULT_OK)
    {
        return result;
    }

    char tmp[CM_PATH_MAX];
    size_t len = strnlen(path, CM_PATH_MAX);
    memcpy(tmp, path, len + 1);

    // Recursively create intermediate directories
    for (char *p = tmp + 1; *p; p++)
    {
        if (*p == '/'
#ifdef _WIN32
            || *p == '\\'
#endif
        )
        {
            char sep = *p;
            *p = '\0';
            if (tsInternalMkdir(tmp) == -1 && errno != EEXIST)
            {
                return CM_RESULT_FAIL_TO_CREATE_DIR;
            }
            *p = sep;
        }
    }

    if (tsInternalMkdir(tmp) == -1 && errno != EEXIST)
    {
        return CM_RESULT_FAIL_TO_CREATE_DIR;
    }

    return CM_RESULT_OK;
}

bool cmInternalFileExists(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file)
    {
        fclose(file);
        return true;
    }
    return false;
}

int cmInternalDeleteFile(const char *path)
{
    int result = cmInternalValidatePath(path);
    if (result != CM_RESULT_OK)
    {
        return result;
    }

    if (!cmInternalFileExists(path))
    {
        return CM_RESULT_FILE_NOT_FOUND;
    }

    if (remove(path) != 0)
    {
        return CM_RESULT_FAIL_TO_DELETE_FILE;
    }

    return CM_RESULT_OK;
}

int cmInternalDeleteDir(const char *path)
{
    int result = cmInternalValidatePath(path);
    if (result != CM_RESULT_OK)
    {
        return result;
    }

    if (!cmInternalDirExists(path))
    {
        return CM_RESULT_DIR_NOT_FOUND;
    }

#ifdef _WIN32
    if (_rmdir(path) != 0)
#else
    if (rmdir(path) != 0)
#endif
    {
        return CM_RESULT_FAIL_TO_DELETE_DIR;
    }

    return CM_RESULT_OK;
}
