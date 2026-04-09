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
// Include
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syslimits.h>

#include "CommonInternal.h"
#include "CommonInternalMessages.h"

#include "LogInternal.h"


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

// Start Related

bool cmInternalIsRunning(cmIsRunningFn cmIsRunning, const char *module, const char *fnName)
{
    if (!cmIsRunning())
    {
        lgInternalLog(ERROR, module, CAUSE_NOT_RUNNING, fnName,CONSEQ_ABORTED);
        return false;
    }

    return true;
}

// Filesystem

bool cmInternalDirExists(const char *path)
{
    struct stat sb;
    return (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode));
}

int cmInternalCreateDir(const char *path)
{
    if (!path)
        return CM_RESULT_NULL_ARG;
    if (!path[0])
        return CM_RESULT_EMPTY_ARG;

    if (path[0] == '/')
    {
        return CM_RESULT_INVALID_PATH;
    }

    char tmp[PATH_MAX];
    size_t len = strnlen(path, PATH_MAX);
    if (len >= PATH_MAX)
    {
        return CM_RESULT_INVALID_PATH;
    }
    memcpy(tmp, path, len + 1);

    // Reject any bare ".." segment to prevent escaping cwd
    char *seg = tmp;
    for (char *p = tmp; ; p++)
    {
        if (*p == '/' || *p == '\0')
        {
            char saved = *p;
            *p = '\0';
            if (strcmp(seg, "..") == 0)
            {
                return CM_RESULT_INVALID_PATH;
            }
            *p = saved;
            if (saved == '\0')
                break;
            seg = p + 1;
        }
    }

    // Recursively create intermediate directories
    for (char *p = tmp + 1; *p; p++)
    {
        if (*p == '/')
        {
            *p = '\0';
            if (mkdir(tmp, 0755) == -1 && errno != EEXIST)
            {
                return -1;
            }
            *p = '/';
        }
    }

    if (mkdir(tmp, 0755) == -1 && errno != EEXIST)
    {
        return -1;
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

// int cmInternalCreateFile(const char *path)
// {
// }
