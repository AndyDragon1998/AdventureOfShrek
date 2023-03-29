#ifndef INTRO_STATE
#define INTRO_STATE

#include "stateManager.h"
#include <stdbool.h>

void IntroStart(GSGLOBAL* gsGlobal);
void IntroUpdate(GSGLOBAL* gsGlobal);
void IntroDraw(GSGLOBAL* gsGlobal, u64 colour);
void IntroEnd(GSGLOBAL* gsGlobal);

extern StateManager IntroState;

#endif
