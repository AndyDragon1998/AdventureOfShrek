#ifndef OVER_STATE
#define OVER_STATE

#include "stateManager.h"

void OverStart(GSGLOBAL* gsGlobal);
void OverUpdate(GSGLOBAL* gsGlobal);
void OverDraw(GSGLOBAL* gsGlobal, u64 colour);
void OverEnd(GSGLOBAL* gsGlobal);

extern StateManager OverState;

#endif
