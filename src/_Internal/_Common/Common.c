/**
 * @file
 * @brief Implementation of the CommonInternal module.
 *
 * @see CommonInternal.h
 * @see CommonInternalMessages.h
 *
 * @bug No known bugs.
 *
 * @author Vitor Betmann
 * @date 2025-12-30
 * @version 1.0.0
 */

// —————————————————————————————————————————————————————————————————————————————
// Include
// —————————————————————————————————————————————————————————————————————————————

#include "CommonInternal.h"
#include "CommonInternalMessages.h"

#include "LogInternal.h"


// —————————————————————————————————————————————————————————————————————————————
// Functions
// —————————————————————————————————————————————————————————————————————————————

bool cmInternalIsRunning(cmIsRunningFn cmIsRunning, const char *module,
                         const char *fnName)
{
    if (!cmIsRunning())
    {
        lgInternalLog(ERROR, module, CAUSE_NOT_RUNNING, fnName,
                      CONSEQ_ABORTED);
        return false;
    }

    return true;
}
