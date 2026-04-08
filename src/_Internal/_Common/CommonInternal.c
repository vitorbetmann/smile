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

#include "CommonInternal.h"
#include "CommonInternalMessages.h"

#include "LogInternal.h"


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

bool cmInternalIsRunning(cmIsRunningFn cmIsRunning, const char *module, const char *fnName)
{
    if (!cmIsRunning())
    {
        lgInternalLog(ERROR, module, CAUSE_NOT_RUNNING, fnName,CONSEQ_ABORTED);
        return false;
    }

    return true;
}

bool cmInternalDirExists(const char *path)
{
}

int cmInternalCreateDir(const char *path)
{
    // should it create dirs recursively?
}

bool cmInternalFileExists(const char *path)
{
}

int cmInternalCreateFile(const char *path)
{
    // should create dirs in case the path has '/'?
}
