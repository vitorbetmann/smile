/**
 * @file
 * @brief Definitions for shared log messages.
 *
 * @author Vitor Betmann
 */


#ifndef SMILE_COMMON_INTERNAL_MESSAGES_H
#define SMILE_COMMON_INTERNAL_MESSAGES_H


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Causes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

// Infos
#define CAUSE_MODULE_STARTED  "Module Started"
#define CAUSE_MODULE_STOPPED  "Module Stopped"
#define CAUSE_FILE_CREATED "File Created"
#define CAUSE_FILE_DELETED "File Deleted"
#define CAUSE_DIR_CREATED "Directory Created"
#define CAUSE_DIR_DELETED "Directory Deleted"
// Warnings
#define CAUSE_ALREADY_RUNNING  "Module Already Running"
// Errors
#define CAUSE_MEM_ALLOC_FAILED  "Memory Allocation Failed"
#define CAUSE_NOT_RUNNING  "Module Not Running"
#define CAUSE_NULL_ARG  "Null Argument"
#define CAUSE_EMPTY_ARG  "Empty Argument"
#define CAUSE_INVALID_PATH "Invalid Path"
#define CAUSE_INVALID_NAME "Invalid Name"
#define CAUSE_FILE_ALREADY_EXISTS "File Already Exists"
#define CAUSE_DIR_ALREADY_EXISTS "Directory Already Exists"
// Fatals
#define CAUSE_FAIL_TO_CREATE_FILE "Failed To Create File"
#define CAUSE_FAIL_TO_CREATE_DIR "Failed To Create Directory"
#define CAUSE_FAIL_TO_DELETE_FILE "Failed To Delete File"
#define CAUSE_FAIL_TO_DELETE_DIR "Failed To Delete Directory"
// Errors or Fatals
#define CAUSE_FILE_DOES_NOT_EXIST "File Does Not Exist"
#define CAUSE_DIR_DOES_NOT_EXIST "Directory Does Not Exist"


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Consequences
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

// Success
#define CONSEQ_SUCCESSFUL  "Successful"
// Failure
#define CONSEQ_ABORTED  "Aborted"
// Pause
#define CONSEQ_PAUSED "Paused"


#endif
