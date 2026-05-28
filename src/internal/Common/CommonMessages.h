#pragma once

// Log - Causes ————————————————————————————————————————————————————————————————————————————————————

// Infos

static const char CSE_MODULE_START[] = "Module Started";
static const char CSE_MODULE_STOP[] = "Module Stopped";
static const char CSE_FILE_CREATE[] = "File Created";
static const char CSE_FILE_DELETE[] = "File Deleted";
static const char CSE_DIR_CREATE[] = "Directory Created";
static const char CSE_DIR_DELETE[] = "Directory Deleted";

// Warnings

static const char CSE_ALREADY_RUNNING[] = "Module Already Running";

// Errors

static const char CSE_MEM_ALLOC_FAIL[] = "Memory Allocation Failed";
static const char CSE_NOT_RUNNING[] = "Module Not Running";
static const char CSE_NULL_ARG[] = "Null Argument";
static const char CSE_EMPTY_ARG[] = "Empty Argument";
static const char CSE_INVALID_ARG[] = "Invalid Argument";
static const char CSE_INVALID_PATH[] = "Invalid Path";
static const char CSE_FILE_ALREADY_EXISTS[] = "File Already Exists";
static const char CSE_DIR_ALREADY_EXISTS[] = "Directory Already Exists";

// Fatals

static const char CSE_CREATE_FILE_FAIL[] = "Failed To Create File";
static const char CSE_CREATE_DIR_FAIL[] = "Failed To Create Directory";
static const char CSE_DEL_FILE_FAIL[] = "Failed To Delete File";
static const char CSE_DEL_DIR_FAIL[] = "Failed To Delete Directory";

// Both Errors and Fatals

static const char CSE_FILE_NOT_EXISTS[] = "File Does Not Exist";
static const char CSE_DIR_NOT_EXISTS[] = "Directory Does Not Exist";

// Log - Consequences ——————————————————————————————————————————————————————————————————————————————

// Success

static const char CSQ_SUCCESS[] = "Successful";

// Failure

static const char CSQ_ABORT[] = "Aborted";

// Pause

static const char CSQ_PAUSE[] = "Paused";
