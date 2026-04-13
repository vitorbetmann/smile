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
#include "Test.h"


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Variables
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#ifdef GS_TESTING
bool gsTestUserConfirms = false;
#endif

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
    "  Note: The scene must have at least 1 callback\n"
    "  Note: <SceneName> must start with a letter or underscore, contain only letters, digits, underscores, or spaces, and must not exceed 64 characters\n"
    "  Note: <dir> is resolved relative to the current working directory, may not contain '..' segments\n"
    "  Note: Neither path (i.e., `<dir>/<SceneName>.c` or `<dir>/<SceneName>.h`) must exceed 256 characters\n";


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

bool gsPrivatePrompt(const char *prompt);

bool gsPrivateYesNoPrompt(const char *prompt);

void gsPrivateWriteSrc(FILE *f, const gsInternalArgs *args);

void gsPrivateWriteHeader(FILE *f, const gsInternalArgs *args);


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions - Internal
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

int gsInternalSanitizeName(char *buf, size_t bufSize, const char *name)
{
    if (!buf || !name)
    {
        return RES_NULL_ARG;
    }
    if (!name[0])
    {
        return RES_EMPTY_ARG;
    }

    const char *src = name;

    while (*src && isspace((unsigned char)*src))
    {
        src++;
    }

    if (!*src)
    {
        return RES_EMPTY_ARG;
    }

    if (!isalpha((unsigned char)*src) && *src != '_')
    {
        return RES_INVALID_ARG;
    }

    size_t out = 0;
    bool capitalizeNext = false;

    while (*src)
    {
        if (isspace((unsigned char)*src))
        {
            while (*src && isspace((unsigned char)*src))
            {
                src++;
            }

            if (!*src)
            {
                break;
            }

            capitalizeNext = true;
        }
        else if (isalnum((unsigned char)*src) || *src == '_')
        {
            if (out + 1 >= bufSize)
            {
                return RES_INVALID_ARG;
            }
            buf[out++] = capitalizeNext ? (char)toupper((unsigned char)*src) : *src;
            capitalizeNext = false;
            src++;
        }
        else
        {
            return RES_INVALID_ARG;
        }
    }

    buf[out] = '\0';
    return RES_OK;
}

void gsInternalFatalHandler(void)
{
    printf("%s", USAGE);
#ifndef GS_TESTING
    exit(1);
#endif
}


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions - Private
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

bool gsPrivatePrompt(const char *prompt)
{
#ifdef GS_TESTING
    (void)prompt;
    return gsTestUserConfirms;
#endif
    return gsPrivateYesNoPrompt(prompt);
}

bool gsPrivateYesNoPrompt(const char *prompt)
{
    printf("%s (Y = ok | N = quit): ", prompt);
    char answer;
    scanf(" %c", &answer);
    return answer == 'Y' || answer == 'y';
}

void gsPrivateWriteSrc(FILE *f, const gsInternalArgs *args)
{
    if (args->addSection)
    {
        fprintf(f, GS_SECTION_DIV "\n// Includes\n" GS_SECTION_DIV "\n");
        fprintf(f, "\n#include <SceneManager.h>\n\n#include \"%s.h\"\n", args->sceneName);
        fprintf(f, "\n\n" GS_SECTION_DIV "\n// Defines\n" GS_SECTION_DIV "\n");
        fprintf(f, "\n\n" GS_SECTION_DIV "\n// Data Types\n" GS_SECTION_DIV "\n");
        fprintf(f, "\n\n" GS_SECTION_DIV "\n// Prototypes\n" GS_SECTION_DIV "\n");
        fprintf(f, "\n\n" GS_SECTION_DIV "\n// Variables\n" GS_SECTION_DIV "\n");
        fprintf(f, "\n\n" GS_SECTION_DIV "\n// Functions\n" GS_SECTION_DIV "\n");
    }
    else
    {
        fprintf(f, "#include <SceneManager.h>\n\n#include \"%s.h\"\n", args->sceneName);
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

void gsPrivateWriteHeader(FILE *f, const gsInternalArgs *args)
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


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions - Main
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

int gsInternalRun(int argc, char *argv[])
{
    if (argc == 1)
    {
        lgInternalLog(ERROR, ORIGIN, CSE_EMPTY_ARG, ORIGIN, CSQ_ABORT);
        return RES_EMPTY_ARG;
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        printf("%s", HELP);
        return 0;
    }

    if (argv[1][0] == '-' || strlen(argv[1]) > GS_NAME_MAX)
    {
        lgInternalLogWithArg(ERROR, ORIGIN, CSE_INVALID_ARG, argv[1], ORIGIN, CSQ_ABORT);
        return RES_INVALID_ARG;
    }

    char sanitizedName[GS_NAME_MAX];
    if (gsInternalSanitizeName(sanitizedName, GS_NAME_MAX, argv[1]) != RES_OK)
    {
        lgInternalLogWithArg(ERROR, ORIGIN, CSE_INVALID_ARG, argv[1], ORIGIN, CSQ_ABORT);
        return RES_INVALID_ARG;
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
                lgInternalLogWithArg(ERROR, ORIGIN, CAUSE_FLAG_REQUIRES_PATH_ARG, argv[i], ORIGIN, CSQ_ABORT);
                return RES_EMPTY_ARG;
            }
            if (cmValidatePath(argv[i + 1]) != RES_OK)
            {
                lgInternalLogWithArg(ERROR, ORIGIN, CSE_INVALID_PATH, argv[i + 1], ORIGIN, CSQ_ABORT);
                return RES_INVALID_PATH;
            }
            args.srcPath = argv[++i];
        }
        else if (strcmp(argv[i], "--header-in") == 0 || strcmp(argv[i], "-hi") == 0)
        {
            if (i + 1 >= argc)
            {
                lgInternalLogWithArg(ERROR, ORIGIN, CAUSE_FLAG_REQUIRES_PATH_ARG, argv[i], ORIGIN, CSQ_ABORT);
                return RES_EMPTY_ARG;
            }
            if (cmValidatePath(argv[i + 1]) != RES_OK)
            {
                lgInternalLogWithArg(ERROR, ORIGIN, CSE_INVALID_PATH, argv[i + 1], ORIGIN, CSQ_ABORT);
                return RES_INVALID_PATH;
            }
            args.includePath = argv[++i];
        }
        else
        {
            lgInternalLogWithArg(ERROR, ORIGIN, CAUSE_INVALID_FLAG, argv[i], ORIGIN, CSQ_ABORT);
            return RES_INVALID_FLAG;
        }
    }

    if (args.noEnter && args.noDraw && args.noUpdate && args.noExit)
    {
        lgInternalLog(ERROR, ORIGIN, CAUSE_NO_CALLBACKS, ORIGIN, CSQ_ABORT);
        return RES_NO_CALLBACKS;
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
        lgInternalLog(FATAL, ORIGIN, CSE_INVALID_PATH, ORIGIN, CSQ_ABORT);
        return RES_INVALID_PATH;
    }

    // Check if they exist
    if (!cmDirExists(args.srcPath))
    {
        lgInternalLogWithArg(WARNING, ORIGIN, CSE_DIR_NOT_EXISTS, args.srcPath, ORIGIN, CSQ_PAUSE);
        char buf[512];
        snprintf(buf, sizeof(buf), "Create directory: '%s'?", args.srcPath);
        if (!gsPrivatePrompt(buf))
        {
            lgInternalLogWithArg(ERROR, ORIGIN, CSE_DIR_NOT_EXISTS, args.srcPath, ORIGIN, CSQ_ABORT);
            return RES_USER_ABORT;
        }
        createSrcDir = true;
    }
    if (!cmDirExists(args.includePath))
    {
        lgInternalLogWithArg(WARNING, ORIGIN, CSE_DIR_NOT_EXISTS, args.includePath, ORIGIN, CSQ_PAUSE);
        char buf[512];
        snprintf(buf, sizeof(buf), "Create directory: '%s'?", args.includePath);
        if (!gsPrivatePrompt(buf))
        {
            lgInternalLogWithArg(ERROR, ORIGIN, CSE_DIR_NOT_EXISTS, args.includePath, ORIGIN, CSQ_ABORT);
            return RES_USER_ABORT;
        }
        createIncludeDir = true;
    }

    // Handling system specific path should be Common's job
    char srcBuf[CM_PATH_MAX];
    snprintf(srcBuf, sizeof(srcBuf), "%s/%s.c", args.srcPath, args.sceneName);
    if (cmFileExists(srcBuf))
    {
        lgInternalLogWithArg(WARNING, ORIGIN, CSE_FILE_ALREADY_EXISTS, srcBuf, ORIGIN, CSQ_PAUSE);
        char buf[2 * CM_PATH_MAX];
        snprintf(buf, sizeof(buf), "Overwrite '%s'? (this may be irreversible)", srcBuf);
        if (!gsPrivatePrompt(buf))
        {
            lgInternalLogWithArg(ERROR, ORIGIN, CSE_FILE_ALREADY_EXISTS, args.srcPath, ORIGIN, CSQ_ABORT);
            return RES_USER_ABORT;
        }
    }

    char includeBuf[CM_PATH_MAX];
    snprintf(includeBuf, sizeof(includeBuf), "%s/%s.h", args.includePath, args.sceneName);
    if (cmFileExists(includeBuf))
    {
        lgInternalLogWithArg(WARNING, ORIGIN, CSE_FILE_ALREADY_EXISTS, includeBuf, ORIGIN, CSQ_PAUSE);
        char buf[2 * CM_PATH_MAX];
        snprintf(buf, sizeof(buf), "Overwrite '%s'? (this may be irreversible)", includeBuf);
        if (!gsPrivatePrompt(buf))
        {
            lgInternalLogWithArg(ERROR, ORIGIN, CSE_FILE_ALREADY_EXISTS, includeBuf, ORIGIN, CSQ_ABORT);
            return RES_USER_ABORT;
        }
    }


    // Create directories if needed
    if (createSrcDir)
    {
        if (cmCreateDir(args.srcPath) != RES_OK)
        {
            lgInternalLogWithArg(FATAL, ORIGIN, CSE_CREATE_DIR_FAIL, args.srcPath, ORIGIN, CSQ_ABORT);
            return RES_CREATE_DIR_FAIL;
        }
        lgInternalLogWithArg(INFO, ORIGIN, CSE_DIR_CREATE, args.srcPath, ORIGIN, CSQ_SUCCESS);
    }
    if (createIncludeDir)
    {
        if (cmCreateDir(args.includePath) != RES_OK)
        {
            lgInternalLogWithArg(FATAL, ORIGIN, CSE_CREATE_DIR_FAIL, args.includePath, ORIGIN, CSQ_ABORT);
            return RES_CREATE_DIR_FAIL;
        }
        lgInternalLogWithArg(INFO, ORIGIN, CSE_DIR_CREATE, args.includePath, ORIGIN, CSQ_SUCCESS);
    }

    FILE *srcFile = tsFopen(srcBuf, "w");
    if (!srcFile)
    {
        lgInternalLogWithArg(FATAL, ORIGIN, CSE_CREATE_FILE_FAIL, srcBuf, ORIGIN, CSQ_ABORT);
        return RES_CREATE_FILE_FAIL;
    }
    gsPrivateWriteSrc(srcFile, &args);
    fclose(srcFile);
    lgInternalLogWithArg(INFO, ORIGIN, CSE_FILE_CREATE, srcBuf, ORIGIN, CSQ_SUCCESS);

    FILE *includeFile = tsFopen(includeBuf, "w");
    if (!includeFile)
    {
        lgInternalLogWithArg(FATAL, ORIGIN, CSE_CREATE_FILE_FAIL, includeBuf, ORIGIN, CSQ_ABORT);
        return RES_CREATE_FILE_FAIL;
    }
    gsPrivateWriteHeader(includeFile, &args);
    fclose(includeFile);

    lgInternalLogWithArg(INFO, ORIGIN, CSE_FILE_CREATE, includeBuf, ORIGIN, CSQ_SUCCESS);
    return RES_OK;
}

#ifndef GS_TESTING
int main(int argc, char *argv[])
{
    lgSetFatal(gsInternalFatalHandler);
    return gsInternalRun(argc, argv) == RES_OK ? 0 : 1;
}
#endif
