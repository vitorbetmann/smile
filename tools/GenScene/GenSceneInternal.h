/**
 * @file
 * @brief Declarations of internal data types and functions for the
 *        GenScene tool.
 *
 * @see GenScene.c
 *
 * @author Vitor Betmann
 */


#ifndef SMILE_GEN_SCENE_INTERNAL_H
#define SMILE_GEN_SCENE_INTERNAL_H


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Defines
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#define DEFAULT_SRC_DIR     "src"
#define DEFAULT_INCLUDE_DIR "include"
#define GS_NAME_MAX         64
#define GS_SECTION_DIV      "// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————"


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Data Types
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Result codes for the GenScene tool.
 *
 * @note GenScene-specific failures cover the following range: `-100..-199`.
 *
 * @see  src/internal/_Common/CommonInternal.h for common result codes
 *
 * @author Vitor Betmann
 */
typedef enum
{
    GS_RESULT_INVALID_FLAG = -100,
    GS_RESULT_NO_CALLBACKS = -101,
    GS_RESULT_USER_ABORTED = -102,
} gsInternalResult;

/**
 * @brief Holds all parsed command-line arguments and options for the GenScene tool.
 *
 * @author Vitor Betmann
 */
typedef struct
{
    char *sceneName;
    char *srcPath;
    char *includePath;

    bool addSection;
    bool noEnter;
    bool noUpdate;
    bool noDraw;
    bool noExit;
} gsInternalArgs;


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Sanitizes a raw scene name into camelCase.
 *
 * Strips leading and trailing whitespace, converts word boundaries (spaces)
 * to uppercase letters, and validates that the result contains only
 * alphanumeric characters and underscores.
 *
 * @param buf     Output buffer for the sanitized name (null-terminated).
 * @param bufSize Capacity of @p buf in bytes.
 * @param name    Raw input name to sanitize.
 *
 * @return `CM_RESULT_OK` on success, or a negative error code on failure.
 *
 * @note Fails if: @p buf or @p name is NULL (`CM_RESULT_NULL_ARG`); @p name is
 *       empty or all whitespace (`CM_RESULT_EMPTY_ARG`); the first character is
 *       not alphabetic, or the name contains characters other than letters,
 *       digits, underscores, or spaces, or the sanitized result exceeds
 *       @p bufSize (`CM_RESULT_INVALID_NAME`).
 *
 * @author Vitor Betmann
 */
int gsInternalSanitizeName(char *buf, size_t bufSize, const char *name);

/**
 * @brief Fatal handler for the GenScene tool.
 *
 * Prints usage instructions to stdout. When not compiled with `GS_TESTING`,
 * terminates the program with `exit(1)`.
 *
 * @note Side effects: in non-testing builds this function does not return.
 *
 * @author Vitor Betmann
 */
void gsInternalFatalHandler(void);

/**
 * @brief Entry point for the GenScene tool.
 *
 * Parses command-line arguments, validates the scene name and directory paths,
 * prompts the user for confirmation before creating directories or overwriting
 * files, then generates a `.c` source file and a `.h` header file containing
 * stubbed scene-lifecycle callbacks.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 *
 * @return `CM_RESULT_OK` on success, or a negative error code on failure.
 *
 * @note Fails if: no scene name is provided (`CM_RESULT_EMPTY_ARG`); the name
 *       is invalid (`CM_RESULT_INVALID_NAME`); a path is invalid or the
 *       combined path exceeds `CM_PATH_MAX` (`CM_RESULT_INVALID_PATH`); an
 *       unknown flag is supplied (`GS_RESULT_INVALID_FLAG`); all callbacks are
 *       disabled (`GS_RESULT_NO_CALLBACKS`); the user declines a prompt
 *       (`GS_RESULT_USER_ABORTED`); a directory cannot be created
 *       (`CM_RESULT_FAIL_TO_CREATE_DIR`); or a file cannot be written
 *       (`CM_RESULT_FAIL_TO_CREATE_FILE`).
 * @note Side effects: may create directories and files on the filesystem;
 *       reads user input interactively via `scanf`.
 *
 * @author Vitor Betmann
 */
int gsInternalRun(int argc, char *argv[]);

#ifdef GS_TESTING
extern bool gsTestUserConfirms;
#endif


#endif
