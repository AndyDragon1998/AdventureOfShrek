/*****************************************************************************/
/*  Автор         		: Дракончик			                                 */
/*  Приставка     		: Sony PlayStation 2					             */
/*  ОС			  		: OSDSYS											 */
/*  Язык	      		: GNU C                                              */
/*                                                                           */
/*  Содержимое файла	: Управление стадии игри		                     */
/*  Атрибут файла		: SOURCE                                             */
/*  Имя файла     		: stateManager.c	                                 */
/*                                                                           */
/*****************************************************************************/
#include "include/stateManager.h"

void StateMachineStart(StateMachine* machine, StateManager* state, GSGLOBAL* gsGlobal)
{
    if (state != 0 && machine != 0)
    {
        machine->ChangeTo = 0;
        machine->TransisionOutFrames = -1;
        machine->TransisionInFrames = -1;

        machine->CurrentState = state;
        machine->CurrentState->Start(gsGlobal);
    }
}


void StateMachineChange(StateMachine* machine, StateManager* state, GSGLOBAL* gsGlobal)
{
    if (state != 0 && machine != 0)
    {
        machine->ChangeTo = 0;

        machine->CurrentState->End(gsGlobal);

        machine->CurrentState = state;

        machine->CurrentState->Start(gsGlobal);
    }
}


void StateMachineUpdate(StateMachine* machine, GSGLOBAL* gsGlobal)
{
    if (machine != 0)
    {
        machine->CurrentState->Update(gsGlobal);
    }
}


void StateMachineDraw(StateMachine* machine, GSGLOBAL* gsGlobal, u64 colour)
{
    if (machine != 0)
    {
        machine->CurrentState->Draw(gsGlobal, colour);
    }
}
