#pragma once

// Includes ————————————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>

// Data Types ——————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief GenScene-exclusive result codes; range -100..-102.
 */
typedef enum
{
    RES_INVALID_FLAG = -100, /**< An unrecognized CLI flag was passed. */
    RES_NO_CALLBACKS = -101, /**< All callbacks were disabled; at least one is required. */
    RES_USER_ABORT = -102,   /**< User declined a confirmation prompt. */
} gsInternalResult;

/**
 * @brief Arguments populated by gsInternalRun for the file-writing helpers.
 */
typedef struct
{
    const char *sceneName;   /**< Sanitized scene name; stack lifetime tied to gsInternalRun. */
    const char *srcPath;     /**< .c output directory; defaults to "src". */
    const char *includePath; /**< .h output directory; defaults to "include". */

    bool addSection; /**< Add section-header dividers to generated files. */
    bool noEnter;    /**< Omit the Enter callback. */
    bool noUpdate;   /**< Omit the Update callback. */
    bool noDraw;     /**< Omit the Draw callback. */
    bool noExit;     /**< Omit the Exit callback. */
} gsInternalArgs;

// Prototypes ——————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Sanitizes a raw scene name into a valid C identifier, trimming whitespace and
 *        converting spaces to camelCase.
 *
 * @param buf     Output buffer for the sanitized name.
 * @param bufSize Capacity of buf in bytes, including the null terminator.
 * @param name    Raw scene name from the command line.
 * @return RES_OK, RES_NULL_ARG, RES_EMPTY_ARG, or RES_INVALID_ARG.
 */
int gsInternalSanitizeName(char *buf, size_t bufSize, const char *name);

/**
 * @brief Prints USAGE and calls exit(1); registered with lgSetFatal() in main.
 *        Under GS_TESTING, exit is compiled out.
 */
void gsInternalFatalHandler(void);

/**
 * @brief Parses argv, sanitizes the name, prompts before filesystem changes, and generates
 *        the scene files.
 *
 * @param argc Forwarded from main.
 * @param argv Forwarded from main.
 * @return RES_OK on success, or the first negative result code encountered.
 */
int gsInternalRun(int argc, char *argv[]);

// Variables  ——————————————————————————————————————————————————————————————————————————————————————

// Constant

/** @brief Max scene name length in bytes; also the sanitization buffer size. */
static constexpr int GS_NAME_MAX = 64;

/** @brief Prefix of a section-divider line. */
static constexpr char GS_SECTION_DIV[] = "// Includes ";

// Mutable

#ifdef GS_TESTING
/** @brief Replaces stdin reads in gsPrivatePrompt to control confirmation branches in tests. */
extern bool gsTestUserConfirms;
#endif