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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CommonInternal.h"
#include "CommonInternalMessages.h"
#include "GenSceneInternal.h"
#include "GenSceneMessages.h"
#include "Log.h"
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
    "  -h,  --help                Show this message (only works as first flag)\n"
    "\n"
    "  -as, --add-sections        Adds smile-style section headers for code organization\n"
    "\n"
    "  -ne, --no-enter            Omit the enter callback\n"
    "  -nu, --no-update           Omit the update callback\n"
    "  -nd, --no-draw             Omit the draw callback\n"
    "  -nx, --no-exit             Omit the exit callback\n"
    "\n"
    "  -si, --source-in <dir>     Outputs the .c file to <dir> (default: src/)\n"
    "  -hi, --header-in <dir>     Outputs the .h file to <dir> (default: include/)\n"
    "\n"
    "  Note: Scene must have at least 1 callback\n"
    "  Note: <SceneName> must start with a letter or underscore, contain only letters, digits, underscores, or spaces (spaces become '_'), and must not exceed 64 characters\n"
    "  Note: <dir> is resolved relative to the current working directory, may not contain '..' segments, and must not exceed 256 characters\n";


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Variables
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

static gsPromptFn gsCurrentPrompt = gsInternalYesNoPrompt;


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions - Internal
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void gsSetPrompt(gsPromptFn fn)
{
    gsCurrentPrompt = fn ? fn : gsInternalYesNoPrompt;
}

bool gsInternalYesNoPrompt(const char *prompt)
{
    printf("%s (Y = ok | N = quit): ", prompt);
    char answer;
    scanf(" %c", &answer);
    return answer == 'Y' || answer == 'y';
}

void gsInternalFatalHandler(void)
{
    printf("%s", USAGE);
    exit(1);
}

void gsInternalWriteHeader(FILE *f, const gsInternalArgs *args)
{
    char upperName[GS_NAME_MAX + 1];
    size_t len = strlen(args->sceneName);
    for (size_t i = 0; i < len; i++)
        upperName[i] = (char)toupper((unsigned char)args->sceneName[i]);
    upperName[len] = '\0';

    fprintf(f, "#ifndef %s_H\n", upperName);
    fprintf(f, "#define %s_H\n", upperName);

    if (args->addSection)
    {
        fprintf(f, "\n\n" GS_SECTION_DIV "\n// Includes\n" GS_SECTION_DIV "\n");
        fprintf(f, "\n\n" GS_SECTION_DIV "\n// Defines\n" GS_SECTION_DIV "\n");
        fprintf(f, "\n\n" GS_SECTION_DIV "\n// Data Types\n" GS_SECTION_DIV "\n");
        fprintf(f, "\n\n" GS_SECTION_DIV "\n// Prototypes\n" GS_SECTION_DIV "\n");
    }
    else
    {
        fprintf(f, "\n");
    }

    if (!args->noEnter)
        fprintf(f, "\nvoid %sEnter(void *args);\n", args->sceneName);
    if (!args->noUpdate)
        fprintf(f, "\nvoid %sUpdate(float dt);\n", args->sceneName);
    if (!args->noDraw)
        fprintf(f, "\nvoid %sDraw(void);\n", args->sceneName);
    if (!args->noExit)
        fprintf(f, "\nvoid %sExit(void);\n", args->sceneName);

    if (args->addSection)
        fprintf(f, "\n\n" GS_SECTION_DIV "\n// Variables\n" GS_SECTION_DIV "\n");

    fprintf(f, "\n\n#endif\n");
}

void gsInternalWriteSrc(FILE *f, const gsInternalArgs *args)
{
    if (args->addSection)
    {
        fprintf(f, GS_SECTION_DIV "\n// Includes\n" GS_SECTION_DIV "\n");
        fprintf(f, "\n#include \"%s.h\"\n", args->sceneName);
        fprintf(f, "\n\n" GS_SECTION_DIV "\n// Defines\n" GS_SECTION_DIV "\n");
        fprintf(f, "\n\n" GS_SECTION_DIV "\n// Data Types\n" GS_SECTION_DIV "\n");
        fprintf(f, "\n\n" GS_SECTION_DIV "\n// Prototypes\n" GS_SECTION_DIV "\n");
        fprintf(f, "\n\n" GS_SECTION_DIV "\n// Variables\n" GS_SECTION_DIV "\n");
        fprintf(f, "\n\n" GS_SECTION_DIV "\n// Functions\n" GS_SECTION_DIV "\n");
    }
    else
    {
        fprintf(f, "#include \"%s.h\"\n", args->sceneName);
    }

    if (!args->noEnter)
        fprintf(f, "\nvoid %sEnter(void *args)\n{\n    // TODO\n}\n", args->sceneName);
    if (!args->noUpdate)
        fprintf(f, "\nvoid %sUpdate(float dt)\n{\n    // TODO\n}\n", args->sceneName);
    if (!args->noDraw)
        fprintf(f, "\nvoid %sDraw(void)\n{\n    // TODO\n}\n", args->sceneName);
    if (!args->noExit)
        fprintf(f, "\nvoid %sExit(void)\n{\n    // TODO\n}\n", args->sceneName);
}


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Main
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

int gsInternalRun(int argc, char *argv[])
{
    if (argc == 1)
    {
        lgInternalLog(FATAL, ORIGIN, CAUSE_EMPTY_ARG, ORIGIN, CONSEQ_ABORTED);
        return 1;
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        printf("%s", HELP);
        return 0;
    }

    if (argv[1][0] == '-' || strlen(argv[1]) > GS_NAME_MAX)
    {
        lgInternalLogWithArg(FATAL, ORIGIN, CAUSE_INVALID_NAME, argv[1], ORIGIN, CONSEQ_ABORTED);
        return 1;
    }

    char sanitizedName[GS_NAME_MAX];
    if (cmInternalSanitizeName(sanitizedName, GS_NAME_MAX, argv[1]) != CM_RESULT_OK)
    {
        lgInternalLogWithArg(FATAL, ORIGIN, CAUSE_INVALID_NAME, argv[1], ORIGIN, CONSEQ_ABORTED);
        return 1;
    }

    gsInternalArgs args = {0};
    args.sceneName = sanitizedName;
    bool createSrcDir = false;
    bool createIncludeDir = false;

    for (int i = 2; i < argc; i++)
    {
        if (strcmp(argv[i], "--add-sections") == 0 || strcmp(argv[i], "-as") == 0)
        {
            args.addSection = true;
        }
        else if (strcmp(argv[i], "--no-enter") == 0 || strcmp(argv[i], "-ne") == 0)
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
                lgInternalLogWithArg(FATAL, ORIGIN, CAUSE_FLAG_REQUIRES_PATH_ARG, argv[i], ORIGIN, CONSEQ_ABORTED);
                return 1;
            }
            if (cmInternalValidatePath(argv[i + 1]) != CM_RESULT_OK)
            {
                lgInternalLogWithArg(FATAL, ORIGIN, CAUSE_INVALID_PATH, argv[i + 1], ORIGIN, CONSEQ_ABORTED);
                return 1;
            }
            args.srcPath = argv[++i];
        }
        else if (strcmp(argv[i], "--header-in") == 0 || strcmp(argv[i], "-hi") == 0)
        {
            if (i + 1 >= argc)
            {
                lgInternalLogWithArg(FATAL, ORIGIN, CAUSE_FLAG_REQUIRES_PATH_ARG, argv[i], ORIGIN, CONSEQ_ABORTED);
                return 1;
            }
            if (cmInternalValidatePath(argv[i + 1]) != CM_RESULT_OK)
            {
                lgInternalLogWithArg(FATAL, ORIGIN, CAUSE_INVALID_PATH, argv[i + 1], ORIGIN, CONSEQ_ABORTED);
                return 1;
            }
            args.includePath = argv[++i];
        }
        else
        {
            lgInternalLogWithArg(FATAL, ORIGIN, CAUSE_INVALID_FLAG, argv[i], ORIGIN, CONSEQ_ABORTED);
            return 1;
        }
    }

    if (args.noEnter && args.noDraw && args.noUpdate && args.noExit)
    {
        lgInternalLog(FATAL, ORIGIN, CAUSE_NO_CALLBACKS, ORIGIN, CONSEQ_ABORTED);
        return 1;
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

    if (strlen(args.sceneName) + strlen(args.srcPath) + 4 > CM_PATH_MAX || // +4 because of ".c\0"
        strlen(args.sceneName) + strlen(args.includePath) + 4 > CM_PATH_MAX) // +4 because of ".h\0"
    {
        lgInternalLog(FATAL, ORIGIN, CAUSE_INVALID_PATH, ORIGIN, CONSEQ_ABORTED);
        return 1;
    }

    // Check if they exist
    if (!cmInternalDirExists(args.includePath))
    {
        lgInternalLogWithArg(ERROR, ORIGIN, CAUSE_DIR_DOES_NOT_EXIST, args.includePath, ORIGIN, CONSEQ_PAUSED);
        if (!gsCurrentPrompt("Create directory?"))
        {
            lgInternalLogWithArg(FATAL, ORIGIN, CAUSE_DIR_DOES_NOT_EXIST, args.includePath, ORIGIN, CONSEQ_ABORTED);
            return 1;
        }
        createIncludeDir = true;
    }
    if (!cmInternalDirExists(args.srcPath))
    {
        lgInternalLogWithArg(ERROR, ORIGIN, CAUSE_DIR_DOES_NOT_EXIST, args.srcPath, ORIGIN, CONSEQ_PAUSED);
        if (!gsCurrentPrompt("Create directory?"))
        {
            lgInternalLogWithArg(FATAL, ORIGIN, CAUSE_DIR_DOES_NOT_EXIST, args.srcPath, ORIGIN, CONSEQ_ABORTED);
            return 1;
        }
        createSrcDir = true;
    }

    // The code below doesn't work on Windows atm
    // Handling system specific path should be Common's job
    // But have it work on macOS for now, upgrade later
    char srcBuf[CM_PATH_MAX];
    snprintf(srcBuf, sizeof(srcBuf), "%s/%s.c", args.srcPath, args.sceneName);
    if (cmInternalFileExists(srcBuf))
    {
        lgInternalLogWithArg(ERROR, ORIGIN, CAUSE_FILE_ALREADY_EXISTS, srcBuf, ORIGIN, CONSEQ_PAUSED);
        if (!gsCurrentPrompt("Overwrite it? (this may be irreversible)"))
        {
            lgInternalLogWithArg(FATAL, ORIGIN, CAUSE_FILE_ALREADY_EXISTS, args.srcPath, ORIGIN, CONSEQ_ABORTED);
            return 1;
        }
    }

    char includeBuf[CM_PATH_MAX];
    snprintf(includeBuf, sizeof(includeBuf), "%s/%s.h", args.includePath, args.sceneName);
    if (cmInternalFileExists(includeBuf))
    {
        lgInternalLogWithArg(ERROR, ORIGIN, CAUSE_FILE_ALREADY_EXISTS, includeBuf, ORIGIN, CONSEQ_PAUSED);
        if (!gsCurrentPrompt("Overwrite it? (this may be irreversible)"))
        {
            lgInternalLogWithArg(FATAL, ORIGIN, CAUSE_FILE_ALREADY_EXISTS, includeBuf, ORIGIN, CONSEQ_ABORTED);
            return 1;
        }
    }


    // Create directories if needed
    if (createSrcDir)
    {
        if (cmInternalCreateDir(args.srcPath) != 0)
        {
            lgInternalLogWithArg(FATAL, ORIGIN, CAUSE_FAIL_TO_CREATE_DIR, args.srcPath, ORIGIN, CONSEQ_ABORTED);
            return 1;
        }
        lgInternalLogWithArg(INFO, ORIGIN, CAUSE_DIR_CREATED, args.srcPath, ORIGIN, CONSEQ_SUCCESSFUL);
    }
    if (createIncludeDir)
    {
        if (cmInternalCreateDir(args.includePath) != 0)
        {
            lgInternalLogWithArg(FATAL, ORIGIN, CAUSE_FAIL_TO_CREATE_DIR, args.includePath, ORIGIN, CONSEQ_ABORTED);
            return 1;
        }
        lgInternalLogWithArg(INFO, ORIGIN, CAUSE_DIR_CREATED, args.includePath, ORIGIN, CONSEQ_SUCCESSFUL);
    }

    FILE *srcFile = fopen(srcBuf, "w");
    if (!srcFile)
    {
        lgInternalLogWithArg(FATAL, ORIGIN, CAUSE_FAIL_TO_CREATE_FILE, srcBuf, ORIGIN, CONSEQ_ABORTED);
        return 1;
    }
    gsInternalWriteSrc(srcFile, &args);
    fclose(srcFile);
    lgInternalLogWithArg(INFO, ORIGIN, CAUSE_FILE_CREATED, srcBuf, ORIGIN, CONSEQ_SUCCESSFUL);

    FILE *includeFile = fopen(includeBuf, "w");
    if (!includeFile)
    {
        lgInternalLogWithArg(FATAL, ORIGIN, CAUSE_FAIL_TO_CREATE_FILE, includeBuf, ORIGIN, CONSEQ_ABORTED);
        return 1;
    }
    gsInternalWriteHeader(includeFile, &args);
    fclose(includeFile);

    lgInternalLogWithArg(INFO, ORIGIN, CAUSE_FILE_CREATED, includeBuf, ORIGIN, CONSEQ_SUCCESSFUL);
    return 0;
}


#ifndef GS_TESTING
int main(int argc, char *argv[])
{
    lgSetFatal(gsInternalFatalHandler);
    return gsInternalRun(argc, argv);
}
#endif
