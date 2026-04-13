/**
 * @file
 * @brief Definitions for shared log messages.
 *
 * @author Vitor Betmann
 */


#ifndef SMILE_COMMON_MESSAGES_H
#define SMILE_COMMON_MESSAGES_H


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Causes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

// Infos
#define CSE_MODULE_START  "Module Started"
#define CSE_MODULE_STOP  "Module Stopped"
#define CSE_FILE_CREATE "File Created"
#define CSE_FILE_DELETE "File Deleted"
#define CSE_DIR_CREATE "Directory Created"
#define CSE_DIR_DELETE "Directory Deleted"
// Warnings
#define CSE_ALREADY_RUNNING  "Module Already Running"
// Errors
#define CSE_MEM_ALLOC_FAIL  "Memory Allocation Failed"
#define CSE_NOT_RUNNING  "Module Not Running"
#define CSE_NULL_ARG  "Null Argument"
#define CSE_EMPTY_ARG  "Empty Argument"
#define CSE_INVALID_ARG "Invalid Argument"
#define CSE_INVALID_PATH "Invalid Path"
#define CSE_FILE_ALREADY_EXISTS "File Already Exists"
#define CSE_DIR_ALREADY_EXISTS "Directory Already Exists"
// Fatals
#define CSE_CREATE_FILE_FAIL "Failed To Create File"
#define CSE_CREATE_DIR_FAIL "Failed To Create Directory"
#define CSE_DELETE_FILE_FAIL "Failed To Delete File"
#define CSE_DELETE_DIR_FAIL "Failed To Delete Directory"
// Errors or Fatals
#define CSE_FILE_NOT_EXISTS "File Does Not Exist"
#define CSE_DIR_NOT_EXISTS "Directory Does Not Exist"


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Consequences
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

// Success
#define CSQ_SUCCESS  "Successful"
// Failure
#define CSQ_ABORT  "Aborted"
// Pause
#define CSQ_PAUSE "Paused"


#endif
