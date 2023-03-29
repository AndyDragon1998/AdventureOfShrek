#ifndef MENU_STATE
#define MENU_STATE

#include "stateManager.h"

void MenuStart(GSGLOBAL* gsGlobal);
void MenuUpdate(GSGLOBAL* gsGlobal);
void MenuDraw(GSGLOBAL* gsGlobal, u64 colour);
void MenuEnd(GSGLOBAL* gsGlobal);

extern StateManager MenuState;

#endif
