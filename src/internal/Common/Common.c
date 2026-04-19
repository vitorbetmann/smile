/**
 * @file
 * @brief Implementation of the Common module.
 *
 * @see Common.h
 * @see CommonMessages.h
 *
 * @author Vitor Betmann
 */


// —————————————————————————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————————————————————————

// External
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
// Module Related
#include "Common.h"
#include "CommonMessages.h"
// Support
#include "internal/Test/Test.h"
#include "LogInternal.h"


// —————————————————————————————————————————————————————————————————————————————————————————————————
// Functions
// —————————————————————————————————————————————————————————————————————————————————————————————————

// Start Related

bool cmIsRunning(cmIsRunningFn cmIsRunning, const char *module,
                 const char *fnName)
{
    if (!cmIsRunning())
    {
        lgInternalLog(ERROR, module, CSE_NOT_RUNNING, fnName, CSQ_ABORT);
        return false;
    }

    return true;
}

// Filesystem

bool cmDirExists(const char *path)
{
#ifdef _WIN32
    struct _stat sb;
    return _stat(path, &sb) == 0 && sb.st_mode & _S_IFDIR;
#else
    struct stat sb;
    return (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode));
#endif
}


int cmValidatePath(const char *path)
{
    if (!path)
    {
        return RES_NULL_ARG;
    }
    if (!path[0])
    {
        return RES_EMPTY_ARG;
    }

#ifdef _WIN32
    if (path[0] == '/' || path[0] == '\\' || (isalpha((unsigned char)path[0]) && path[1] == ':'))
    {
        return RES_INVALID_PATH;
    }
#else
    if (path[0] == '/')
    {
        return RES_INVALID_PATH;
    }
#endif

    size_t len = strnlen(path, CM_PATH_MAX);
    if (len >= CM_PATH_MAX)
    {
        return RES_INVALID_PATH;
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
                return RES_INVALID_PATH;
            }
            *p = saved;
            if (saved == '\0')
            {
                break;
            }
            seg = p + 1;
        }
    }

    return RES_OK;
}


int cmCreateDir(const char *path)
{
    int result = cmValidatePath(path);
    if (result != RES_OK)
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
            if (tsMkdir(tmp) == -1 && errno != EEXIST)
            {
                return RES_CREATE_DIR_FAIL;
            }
            *p = sep;
        }
    }

    if (tsMkdir(tmp) == -1 && errno != EEXIST)
    {
        return RES_CREATE_DIR_FAIL;
    }

    return RES_OK;
}

bool cmFileExists(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file)
    {
        fclose(file);
        return true;
    }
    return false;
}

int cmDeleteFile(const char *path)
{
    int result = cmValidatePath(path);
    if (result != RES_OK)
    {
        return result;
    }

    if (!cmFileExists(path))
    {
        return RES_FILE_NOT_FOUND;
    }

    if (remove(path) != 0)
    {
        return RES_DEL_FILE_FAIL;
    }

    return RES_OK;
}

int cmDeleteDir(const char *path)
{
    int result = cmValidatePath(path);
    if (result != RES_OK)
    {
        return result;
    }

    if (!cmDirExists(path))
    {
        return RES_DIR_NOT_FOUND;
    }

#ifdef _WIN32
    if (_rmdir(path) != 0)
#else
    if (rmdir(path) != 0)
#endif
    {
        return RES_DEL_DIR_FAIL;
    }

    return RES_OK;
}
