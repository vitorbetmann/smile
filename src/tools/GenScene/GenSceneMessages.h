#pragma once

// Tool Name ———————————————————————————————————————————————————————————————————————————————————————

static const char ORI[] = "GenScene";

// Tool Messages ———————————————————————————————————————————————————————————————————————————————————

static const char USAGE[] =
    "Usage: GenScene <SceneName> [options]\n"
    "Try 'GenScene --help' for more information.\n";

static const char HELP[] =
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

// Log - Causes ————————————————————————————————————————————————————————————————————————————————————

// Fatals

static const char CSE_NO_CALLBACKS[] = "Scene Has No Callbacks";
static const char CSE_FLAG_REQ_PATH_ARG[] = "Flag Requires Path Argument";
static const char CSE_INVALID_FLAG[] = "Invalid Flag";
