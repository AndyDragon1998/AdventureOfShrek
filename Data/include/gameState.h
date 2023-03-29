#ifndef GAME_STATE
#define GAME_STATE

#include "stateManager.h"
#include <stdbool.h>

void GameStart(GSGLOBAL* gsGlobal);
void GameUpdate(GSGLOBAL* gsGlobal);
void GameDraw(GSGLOBAL* gsGlobal, u64 colour);
void GameEnd(GSGLOBAL* gsGlobal);

extern StateManager GameState;

#endif
