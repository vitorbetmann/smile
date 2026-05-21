#pragma once

// Log - Causes ————————————————————————————————————————————————————————————————————————————————————

// Infos

static constexpr char CSE_MODULE_START[] = "Module Started";
static constexpr char CSE_MODULE_STOP[] = "Module Stopped";
static constexpr char CSE_FILE_CREATE[] = "File Created";
static constexpr char CSE_FILE_DELETE[] = "File Deleted";
static constexpr char CSE_DIR_CREATE[] = "Directory Created";
static constexpr char CSE_DIR_DELETE[] = "Directory Deleted";

// Warnings

static constexpr char CSE_ALREADY_RUNNING[] = "Module Already Running";

// Errors

static constexpr char CSE_MEM_ALLOC_FAIL[] = "Memory Allocation Failed";
static constexpr char CSE_NOT_RUNNING[] = "Module Not Running";
static constexpr char CSE_NULL_ARG[] = "Null Argument";
static constexpr char CSE_EMPTY_ARG[] = "Empty Argument";
static constexpr char CSE_INVALID_ARG[] = "Invalid Argument";
static constexpr char CSE_INVALID_PATH[] = "Invalid Path";
static constexpr char CSE_FILE_ALREADY_EXISTS[] = "File Already Exists";
static constexpr char CSE_DIR_ALREADY_EXISTS[] = "Directory Already Exists";

// Fatals

static constexpr char CSE_CREATE_FILE_FAIL[] = "Failed To Create File";
static constexpr char CSE_CREATE_DIR_FAIL[] = "Failed To Create Directory";
static constexpr char CSE_DEL_FILE_FAIL[] = "Failed To Delete File";
static constexpr char CSE_DEL_DIR_FAIL[] = "Failed To Delete Directory";

// Both Errors and Fatals

static constexpr char CSE_FILE_NOT_EXISTS[] = "File Does Not Exist";
static constexpr char CSE_DIR_NOT_EXISTS[] = "Directory Does Not Exist";

// Log - Consequences ——————————————————————————————————————————————————————————————————————————————

// Success

static constexpr char CSQ_SUCCESS[] = "Successful";

// Failure

static constexpr char CSQ_ABORT[] = "Aborted";

// Pause

static constexpr char CSQ_PAUSE[] = "Paused";
