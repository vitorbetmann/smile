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

#include "CommonInternal.h"
#include "CommonInternalMessages.h"
#include "GenSceneInternal.h"
#include "GenSceneMessages.h"
#include "LogInternal.h"


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Variables
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

static const char *USAGE =
    "Usage: GenScene <SceneName> [options]\n"
    "Try 'GenScene --help' for more information.\n";

static const char *HELP =
    "Usage: GenScene <SceneName> [options]\n"
    "\n"
    "Options:\n"
    "  -ne, --no-enter            Omit the enter callback\n"
    "  -nu, --no-update           Omit the update callback\n"
    "  -nd, --no-draw             Omit the draw callback\n"
    "  -nx, --no-exit             Omit the exit callback\n"
    "  -si, --source-in <dir>     Output .c file to this directory (default: src/)\n"
    "  -hi, --header-in <dir>     Output .h file to this directory (default: include/)\n"
    "  -h,  --help                Show this message\n"
    "\n"
    "  Paths must be relative (no leading '/') and may not contain '..' segments.\n";


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions - Internal
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

bool gsInternalYesNoPrompt(const char *prompt)
{
    printf("%s 'n' terminates. (Y/n): ", prompt);
    char answer;
    scanf(" %c", &answer);
    return answer == 'Y' || answer == 'y';
}


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Main
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("%s", USAGE);
        return 1;
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        printf("%s", HELP);
        return 0;
    }

    if (argv[1][0] == '-')
    {
        printf("%s", USAGE);
        return 1;
    }

    gsInternalArgs args = {0};
    args.sceneName = argv[1];
    bool createSrcDir = false;
    bool createIncludeDir = false;

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
        else if (strcmp(argv[i], "--source-in") == 0 || strcmp(argv[i], "-si") == 0)
        {
            if (i + 1 >= argc)
            {
                lgInternalLogWithArg(ERROR, ORIGIN,CAUSE_FLAG_REQUIRES_PATH_ARG, argv[i], __func__, CONSEQ_ABORTED);
                return 1;
            }
            args.srcPath = argv[++i];
        }
        else if (strcmp(argv[i], "--header-in") == 0 || strcmp(argv[i], "-hi") == 0)
        {
            if (i + 1 >= argc)
            {
                lgInternalLogWithArg(ERROR, ORIGIN,CAUSE_FLAG_REQUIRES_PATH_ARG, argv[i], __func__, CONSEQ_ABORTED);
                return 1;
            }
            args.includePath = argv[++i];
        }
        else
        {
            printf("%s", USAGE);
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
        lgInternalLogWithArg(ERROR, ORIGIN, CAUSE_DIR_DOES_NOT_EXIST, args.includePath, __func__, CONSEQ_STOPPED);
        {
            if (!gsInternalYesNoPrompt("Create directory?"))
                return 1;
        }
        createIncludeDir = true;
    }
    if (!cmInternalDirExists(args.srcPath))
    {
        lgInternalLogWithArg(ERROR, ORIGIN, CAUSE_DIR_DOES_NOT_EXIST, args.srcPath, __func__, CONSEQ_STOPPED);
        if (!gsInternalYesNoPrompt("Create directory?"))
        {
            return 1;
        }
        createSrcDir = true;
    }

    // The code below doesn't work on Windows atm
    // Need to sanitize input and make is system agnostic
    // Handling system specific path should be Common's job
    // But have it work on macOS for now, upgrade later
    // char srcBuf[CM_PATH_MAX];
    // snprintf(srcBuf, sizeof(srcBuf), "%s/%s.c", args.srcPath, args.sceneName);
    // if (cmInternalFileExists(srcBuf))
    // {
    //     lgInternalLogWithArg(ERROR, ORIGIN, CAUSE_FILE_ALREADY_EXISTS, argv[1], __func__, CONSEQ_STOPPED);
    //     // if (!gsInternalYesNoPrompt("Overwrite it?"))
    //     // {
    //     return 1;
    //     // }
    // }
    //
    // char includeBuf[CM_PATH_MAX];
    // snprintf(includeBuf, sizeof(includeBuf), "%s/%s.h", args.includePath, args.sceneName);
    // if (cmInternalFileExists(includeBuf))
    // {
    //     lgInternalLogWithArg(ERROR, ORIGIN, CAUSE_FILE_ALREADY_EXISTS, argv[1], __func__, CONSEQ_STOPPED);
    //     // if (!gsInternalYesNoPrompt("Overwrite it?"))
    //     // {
    //     return 1;
    //     // }
    // }


    // Create directories if needed
    if (createSrcDir)
    {
        if (cmInternalCreateDir(args.srcPath) != 0)
        {
            lgInternalLogWithArg(FATAL, ORIGIN, CAUSE_FAIL_TO_CREATE_DIR, args.srcPath, __func__, CONSEQ_ABORTED);
        }
        lgInternalLogWithArg(INFO, ORIGIN, CAUSE_DIR_CREATED, args.srcPath,ORIGIN, CONSEQ_SUCCESSFUL);
    }
    if (createIncludeDir)
    {
        if (cmInternalCreateDir(args.includePath) != 0)
        {
            lgInternalLogWithArg(FATAL, ORIGIN, CAUSE_FAIL_TO_CREATE_DIR, args.includePath, __func__, CONSEQ_ABORTED);
        }
        lgInternalLogWithArg(INFO, ORIGIN, CAUSE_DIR_CREATED, args.includePath,ORIGIN, CONSEQ_SUCCESSFUL);
    }

    // create files in directories
    // write to files according to specifications
    lgInternalLog(INFO, ORIGIN, CAUSE_FILES_CREATED, ORIGIN, CONSEQ_SUCCESSFUL);

    return 0;
}
