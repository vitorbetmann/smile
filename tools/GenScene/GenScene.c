/**
 * @file
 * @brief Implementation of the GenScene tool.
 *
 * @see GenSceneInternal.h
 * @see GenSceneMessages.h
 *
 * @author Vitor Betmann
 */

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <string.h>
#include <sys/syslimits.h>

#include "CommonInternal.h"
#include "CommonInternalMessages.h"
#include "GenSceneInternal.h"
#include "GenSceneMessages.h"
#include "LogInternal.h"


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Defines
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Data Types
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Variables
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions - Public
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions - Internal
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions - Private
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Main
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        // print usage
        return 1;
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        // print help menu
        return 0;
    }

    if (argv[1][0] == '-')
    {
        // error: first argument must be the scene name
        // print usage
        return 1;
    }

    gsInternalArgs args = {0};

    // Remember to sanitize argv[1]
    args.sceneName = argv[1];

    for (int i = 2; i < argc; i++)
    {
        if (strcmp(argv[i], "--no-enter") == 0 || strcmp(argv[i], "-ne") == 0)
        {
            args.noEnter = true;
        }
        else if (strcmp(argv[i], "--no-update") == 0 || strcmp(argv[i], "-nu") == 0)
        {
            args.noUpdate = true;
        }
        else if (strcmp(argv[i], "--no-draw") == 0 || strcmp(argv[i], "-nd") == 0)
        {
            args.noDraw = true;
        }
        else if (strcmp(argv[i], "--no-exit") == 0 || strcmp(argv[i], "-nx") == 0)
        {
            args.noExit = true;
        }
        else if (strcmp(argv[i], "--source-in") == 0 || strcmp(argv[i], "-src") == 0)
        {
            if (i + 1 >= argc)
            {
                // error: --source-in requires a path argument
                return 1;
            }
            args.srcPath = argv[++i];
        }
        else if (strcmp(argv[i], "--header-in") == 0 || strcmp(argv[i], "-hdr") == 0)
        {
            if (i + 1 >= argc)
            {
                // error: --header-in requires a path argument
                return 1;
            }
            args.includePath = argv[++i];
        }
        else
        {
            // print usage
            return 1;
        }
    }

    // if srcPath and includePath are null, set them to the default
    if (!args.includePath)
    {
        args.includePath = DEFAULT_INCLUDE_DIR;
    }
    if (!args.srcPath)
    {
        args.srcPath = DEFAULT_SRC_DIR;
    }

    // Check they exist
    if (!cmInternalDirExists(args.includePath))
    {
        // ask if they wish to create a dir with the given name
        // if not allowed, exit
    }
    if (!cmInternalDirExists(args.srcPath))
    {
        // ask if they wish to create a dir with the given name
        // if not allowed, exit
    }


    // The code below doesn't work on Windows atm
    // Need to sanitize input and make is system agnostic
    // Handling system specific path should be Common's job
    char srcBuf[PATH_MAX];
    snprintf(srcBuf, sizeof(srcBuf), "%s/%s.c", args.srcPath, args.sceneName);
    if (cmInternalFileExists(srcBuf))
    {
        lgInternalLogWithArg(ERROR, ORIGIN, CAUSE_SOURCE_FILE_ALREADY_EXISTS, argv[1], __func__, CONSEQ_ABORTED);
        return 1;
    }

    char includeBuf[PATH_MAX];
    snprintf(srcBuf, sizeof(srcBuf), "%s/%s.h", args.includePath, args.sceneName);
    if (cmInternalFileExists(includeBuf))
    {
        lgInternalLogWithArg(ERROR, ORIGIN, CAUSE_SOURCE_FILE_ALREADY_EXISTS, argv[1], __func__, CONSEQ_ABORTED);
        return 1;
    }


    /*
     * create the files in the correct directories and write files according to specifications
     */
    lgInternalLog(INFO, ORIGIN, CAUSE_FILES_CREATED, ORIGIN, CONSEQ_SUCCESSFUL);


    return 0;
}
