#ifndef TEST_INTERNAL_H
#define TEST_INTERNAL_H

// --------------------------------------------------
// Other defines
// --------------------------------------------------

#define SM_COMP_NAME(name1, name2) strcmp(name1, name2) == 0
#define TEST_PASS(funcName) printf("\t[PASS] %s\n", funcName)

#endif