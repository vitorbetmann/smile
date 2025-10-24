#ifndef SMILE_STATE_MACHINE_TEST_H
#define SMILE_STATE_MACHINE_TEST_H

// --------------------------------------------------
// Includes
// --------------------------------------------------

#include "../../src/StateMachine/StateMachineInternal.h"

// --------------------------------------------------
// Other Defines
// --------------------------------------------------

#define STRESS_ITERATIONS 1000
#define IDEMPOTENT_ITERATIONS 3

// --------------------------------------------------
// Data types
// --------------------------------------------------

typedef struct
{
    bool hasEntered, hasEnteredArgs;
    int enteredTimes;

    bool hasUpdated;
    bool hasDrawn;

    bool hasExited;
    int exitedTimes;
} MockData;

typedef struct
{
    bool flag;
} MockStateArgs;

// --------------------------------------------------
// variables
// --------------------------------------------------

static unsigned int MULTIPLE_STATES = 1000;
static float mockDt = 0.016;
MockData md;
static InternalState mockState = {.name = "mockState"};

// --------------------------------------------------
// Mock Functions
// --------------------------------------------------

void mockEnter(void *args)
{
    md.hasEntered = true;
    md.enteredTimes++;

    if (args)
    {
        MockStateArgs *msa = args;
        md.hasEnteredArgs = msa->flag;
    }
}

void mockUpdate(float dt)
{
    md.hasUpdated = true;
}

void mockDraw(void)
{
    md.hasDrawn = true;
}

void mockExit(void)
{
    md.hasExited = true;
    md.exitedTimes++;
}

#endif // #ifndef SMILE_STATE_MACHINE_TEST_H

