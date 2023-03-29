/*****************************************************************************/
/*  Автор         		: Дракончик				                             */
/*  Приставка     		: Sony PlayStation 2					             */
/*  ОС			  		: OSDSYS											 */
/*  Язык	      		: GNU C                                              */
/*                                                                           */
/*  Содержимое файла	: Стадия завершение Игри	                         */
/*  Атрибут файла		: SOURCE                                             */
/*  Имя файла     		: overState.c	                                     */
/*                                                                           */
/*****************************************************************************/

#include "include/gameState.h"
#include "include/menuState.h"
#include "include/overState.h"
#include "include/introState.h"
#include "include/pad.h"

#include "include/background.h"
#include <stdlib.h>

#include "extern/gameOver.ext"
#include "define/gameOver.def"

GSTEXTURE BackgroundOverImage;


void OverStart(GSGLOBAL* gsGlobal)
{
	char tempy[4096];
	strcpy(tempy, relativePath);
	strcat(tempy, "/Graphics/Background/background_image2.png");
	gsKit_texture_png(gsGlobal, &BackgroundOverImage,tempy);
}


void OverUpdate(GSGLOBAL* gsGlobal)
{
	if(PlaystationGamePad.START_KEY_TAP)
    	{	
		
		
			StateMachineChange(&GameMachineState, &GameState, gsGlobal);
		
	}
}

void OverDraw(GSGLOBAL* gsGlobal, u64 colour)
{
	gsKit_prim_sprite_texture(gsGlobal, &BackgroundOverImage,0,  // X1
						0,  // Y2
						0.0f,  // U1
						0.0f,  // V1
						BackgroundOverImage.Width * 2, // X2
						BackgroundOverImage.Height * 2, // Y2
						BackgroundOverImage.Width * 2, // U2
						BackgroundOverImage.Height * 2, // V2
						2,
						colour);
	// DrawText(120, 380, "YF:VBNT",2, gsGlobal, &colour);
}

void OverEnd(GSGLOBAL* gsGlobal)
{

}

StateManager OverState =
{
	OverStart,
	OverUpdate,
	OverDraw,
	OverEnd
};
