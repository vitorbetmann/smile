// Includes ————————————————————————————————————————————————————————————————————————————————————————

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal/Common/Common.h"
#include "internal/Common/CommonMessages.h"
#include "GenSceneInternal.h"
#include "GenSceneMessages.h"
#include "Log.h"
#include "LogInternal.h"
#include "internal/Test/Test.h"

// Variables ———————————————————————————————————————————————————————————————————————————————————————

// Constant

static const char DEFAULT_SRC_DIR[] = "src";
static const char DEFAULT_INCLUDE_DIR[] = "include";

// Mutable

#ifdef GS_TESTING
bool gsTestUserConfirms = false;
#endif

// Prototypes ——————————————————————————————————————————————————————————————————————————————————————

bool gsPrivatePrompt(const char *prompt);

bool gsPrivateYesNoPrompt(const char *prompt);

void gsPrivateWriteSection(FILE *f, const char *name);

void gsPrivateWriteSrc(FILE *f, const gsInternalArgs *args);

void gsPrivateWriteHeader(FILE *f, const gsInternalArgs *args);

// Functions - Internal ————————————————————————————————————————————————————————————————————————————

int gsInternalRun(const int argc, char *argv[])
{
    if (argc == 1)
    {
        lgInternalLog(ERROR, ORI, CSE_EMPTY_ARG, ORI, CSQ_ABORT);
        return RES_EMPTY_ARG;
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        printf("%s", HELP);
        return RES_OK;
    }

    if (argv[1][0] == '-' || strlen(argv[1]) > GS_NAME_MAX)
    {
        lgInternalLogWithArg(ERROR, ORI, CSE_INVALID_ARG, argv[1], ORI, CSQ_ABORT);
        return RES_INVALID_ARG;
    }

    char sanitizedName[GS_NAME_MAX];
    if (gsInternalSanitizeName(sanitizedName, GS_NAME_MAX, argv[1]) != RES_OK)
    {
        lgInternalLogWithArg(ERROR, ORI, CSE_INVALID_ARG, argv[1], ORI, CSQ_ABORT);
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
                lgInternalLogWithArg(ERROR, ORI, CSE_FLAG_REQ_PATH_ARG, argv[i], ORI, CSQ_ABORT);
                return RES_EMPTY_ARG;
            }
            if (cmValidatePath(argv[i + 1]) != RES_OK)
            {
                lgInternalLogWithArg(ERROR, ORI, CSE_INVALID_PATH, argv[i + 1], ORI, CSQ_ABORT);
                return RES_INVALID_PATH;
            }
            args.srcPath = argv[++i];
        }
        else if (strcmp(argv[i], "--header-in") == 0 || strcmp(argv[i], "-hi") == 0)
        {
            if (i + 1 >= argc)
            {
                lgInternalLogWithArg(ERROR, ORI, CSE_FLAG_REQ_PATH_ARG, argv[i], ORI, CSQ_ABORT);
                return RES_EMPTY_ARG;
            }
            if (cmValidatePath(argv[i + 1]) != RES_OK)
            {
                lgInternalLogWithArg(ERROR, ORI, CSE_INVALID_PATH, argv[i + 1], ORI, CSQ_ABORT);
                return RES_INVALID_PATH;
            }
            args.includePath = argv[++i];
        }
        else
        {
            lgInternalLogWithArg(ERROR, ORI, CSE_INVALID_FLAG, argv[i], ORI, CSQ_ABORT);
            return RES_INVALID_FLAG;
        }
    }

    if (args.noEnter && args.noDraw && args.noUpdate && args.noExit)
    {
        lgInternalLog(ERROR, ORI, CSE_NO_CALLBACKS, ORI, CSQ_ABORT);
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
        lgInternalLog(FATAL, ORI, CSE_INVALID_PATH, ORI, CSQ_ABORT);
        return RES_INVALID_PATH;
    }

    // Check if they exist
    if (!cmDirExists(args.srcPath))
    {
        lgInternalLogWithArg(WARN, ORI, CSE_DIR_NOT_EXISTS, args.srcPath, ORI, CSQ_PAUSE);
        char buf[512];
        snprintf(buf, sizeof(buf), "Create directory: '%s'?", args.srcPath);
        if (!gsPrivatePrompt(buf))
        {
            lgInternalLogWithArg(ERROR, ORI, CSE_DIR_NOT_EXISTS, args.srcPath, ORI, CSQ_ABORT);
            return RES_USER_ABORT;
        }
        createSrcDir = true;
    }
    if (!cmDirExists(args.includePath))
    {
        lgInternalLogWithArg(WARN, ORI, CSE_DIR_NOT_EXISTS, args.includePath, ORI, CSQ_PAUSE);
        char buf[512];
        snprintf(buf, sizeof(buf), "Create directory: '%s'?", args.includePath);
        if (!gsPrivatePrompt(buf))
        {
            lgInternalLogWithArg(ERROR, ORI, CSE_DIR_NOT_EXISTS, args.includePath, ORI, CSQ_ABORT);
            return RES_USER_ABORT;
        }
        createIncludeDir = true;
    }

    // Handling system specific path should be Common's job
    char srcBuf[CM_PATH_MAX];
    snprintf(srcBuf, sizeof(srcBuf), "%s/%s.c", args.srcPath, args.sceneName);
    if (cmFileExists(srcBuf))
    {
        lgInternalLogWithArg(WARN, ORI, CSE_FILE_ALREADY_EXISTS, srcBuf, ORI, CSQ_PAUSE);
        char buf[2 * CM_PATH_MAX];
        snprintf(buf, sizeof(buf), "Overwrite '%s'? (this may be irreversible)", srcBuf);
        if (!gsPrivatePrompt(buf))
        {
            lgInternalLogWithArg(ERROR, ORI, CSE_FILE_ALREADY_EXISTS, args.srcPath, ORI, CSQ_ABORT);
            return RES_USER_ABORT;
        }
    }

    char includeBuf[CM_PATH_MAX];
    snprintf(includeBuf, sizeof(includeBuf), "%s/%s.h", args.includePath, args.sceneName);
    if (cmFileExists(includeBuf))
    {
        lgInternalLogWithArg(WARN, ORI, CSE_FILE_ALREADY_EXISTS, includeBuf, ORI, CSQ_PAUSE);
        char buf[2 * CM_PATH_MAX];
        snprintf(buf, sizeof(buf), "Overwrite '%s'? (this may be irreversible)", includeBuf);
        if (!gsPrivatePrompt(buf))
        {
            lgInternalLogWithArg(ERROR, ORI, CSE_FILE_ALREADY_EXISTS, includeBuf, ORI, CSQ_ABORT);
            return RES_USER_ABORT;
        }
    }


    // Create directories if needed
    if (createSrcDir)
    {
        if (cmCreateDir(args.srcPath) != RES_OK)
        {
            lgInternalLogWithArg(FATAL, ORI, CSE_CREATE_DIR_FAIL, args.srcPath, ORI, CSQ_ABORT);
            return RES_CREATE_DIR_FAIL;
        }
        lgInternalLogWithArg(INFO, ORI, CSE_DIR_CREATE, args.srcPath, ORI, CSQ_SUCCESS);
    }
    if (createIncludeDir)
    {
        if (cmCreateDir(args.includePath) != RES_OK)
        {
            lgInternalLogWithArg(FATAL, ORI, CSE_CREATE_DIR_FAIL, args.includePath, ORI, CSQ_ABORT);
            return RES_CREATE_DIR_FAIL;
        }
        lgInternalLogWithArg(INFO, ORI, CSE_DIR_CREATE, args.includePath, ORI, CSQ_SUCCESS);
    }

    FILE *srcFile = tsFopen(srcBuf, "w");
    if (!srcFile)
    {
        lgInternalLogWithArg(FATAL, ORI, CSE_CREATE_FILE_FAIL, srcBuf, ORI, CSQ_ABORT);
        return RES_CREATE_FILE_FAIL;
    }
    gsPrivateWriteSrc(srcFile, &args);
    fclose(srcFile);
    lgInternalLogWithArg(INFO, ORI, CSE_FILE_CREATE, srcBuf, ORI, CSQ_SUCCESS);

    FILE *includeFile = tsFopen(includeBuf, "w");
    if (!includeFile)
    {
        lgInternalLogWithArg(FATAL, ORI, CSE_CREATE_FILE_FAIL, includeBuf, ORI, CSQ_ABORT);
        return RES_CREATE_FILE_FAIL;
    }
    gsPrivateWriteHeader(includeFile, &args);
    fclose(includeFile);

    lgInternalLogWithArg(INFO, ORI, CSE_FILE_CREATE, includeBuf, ORI, CSQ_SUCCESS);
    return RES_OK;
}

int gsInternalSanitizeName(char *buf, const size_t bufSize, const char *name)
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

// Functions - Private —————————————————————————————————————————————————————————————————————————————

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

void gsPrivateWriteSection(FILE *f, const char *name)
{
    int dashCount = 96 - (int)strlen(name);
    fprintf(f, "// %s ", name);
    for (int i = 0; i < dashCount; i++)
        fputs("—", f);
    fputc('\n', f);
}

void gsPrivateWriteSrc(FILE *f, const gsInternalArgs *args)
{
    if (args->addSection)
    {
        gsPrivateWriteSection(f, "Includes");
        fprintf(f, "\n#include <SceneManager.h>\n\n#include \"%s.h\"\n", args->sceneName);
        fprintf(f, "\n\n");
        gsPrivateWriteSection(f, "Defines");
        fprintf(f, "\n\n");
        gsPrivateWriteSection(f, "Data Types");
        fprintf(f, "\n\n");
        gsPrivateWriteSection(f, "Prototypes");
        fprintf(f, "\n\n");
        gsPrivateWriteSection(f, "Variables");
        fprintf(f, "\n\n");
        gsPrivateWriteSection(f, "Functions");
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
    fprintf(f, "#pragma once");

    if (args->addSection)
    {
        fprintf(f, "\n\n");
        gsPrivateWriteSection(f, "Includes");
        fprintf(f, "\n\n");
        gsPrivateWriteSection(f, "Defines");
        fprintf(f, "\n\n");
        gsPrivateWriteSection(f, "Data Types");
        fprintf(f, "\n\n");
        gsPrivateWriteSection(f, "Prototypes");
    }
    else
    {
        fprintf(f, "\n");
    }

    if (!args->noEnter)
    {
        fprintf(f, "\nvoid %sEnter(void *args);\n", args->sceneName);
    }
    if (!args->noUpdate)
    {
        fprintf(f, "\nvoid %sUpdate(float dt);\n", args->sceneName);
    }
    if (!args->noDraw)
    {
        fprintf(f, "\nvoid %sDraw(void);\n", args->sceneName);
    }
    if (!args->noExit)
    {
        fprintf(f, "\nvoid %sExit(void);\n", args->sceneName);
    }

    if (args->addSection)
    {
        fprintf(f, "\n");
        gsPrivateWriteSection(f, "Variables");
    }
}

// Functions - Main ————————————————————————————————————————————————————————————————————————————————

#ifndef GS_TESTING
int main(const int argc, char *argv[])
{
    lgSetFatal(gsInternalFatalHandler);
    return gsInternalRun(argc, argv) == RES_OK ? 0 : 1;
}
#endif
