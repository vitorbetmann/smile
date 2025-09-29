#ifndef STATE_MACHINE_TEST_H
#define STATE_MACHINE_TEST_H

// --------------------------------------------------
// Includes
// --------------------------------------------------

#include "../../src/StateMachine/StateMachineInternal.h"

// --------------------------------------------------
// Data types
// --------------------------------------------------

typedef struct {
    bool hasEntered, hasEnteredArgs;
    int enteredTimes;

    bool hasUpdated;
    bool hasDrawn;

    bool hasExited;
    int exitedTimes;
} MockData;

typedef struct {
    bool flag;
} MockStateArgs;

// --------------------------------------------------
// variables
// --------------------------------------------------

static unsigned int MULTIPLE_STATES = 1000;
static float mockDT = 0.016;
MockData md;
static State mockState = {.name = "mockState"};

// --------------------------------------------------
// Mock Functions
// --------------------------------------------------

void mockEnter(void *args) {
    md.hasEntered = true;
    md.enteredTimes++;

    if (args) {
        MockStateArgs *msa = (MockStateArgs *) args;
        md.hasEnteredArgs = msa->flag;
    }
}

void mockUpdate(float dt) { md.hasUpdated = true; }
void mockDraw(void) { md.hasDrawn = true; }

void mockExit(void) {
    md.hasExited = true;
    md.exitedTimes++;
}

#endif
