/*****************************************************************************/
/*  Автор         		: Дракончик				                             */
/*  Приставка     		: Sony PlayStation 2					             */
/*  ОС			  		: OSDSYS											 */
/*  Язык	      		: GNU C                                              */
/*                                                                           */
/*  Содержимое файла	: Стадия логотипов			                         */
/*  Атрибут файла		: SOURCE                                             */
/*  Имя файла     		: introState.c	                                     */
/*                                                                           */
/*****************************************************************************/

#include "include/gameState.h"
#include "include/menuState.h"
#include "include/overState.h"
#include "include/introState.h"
#include <stdlib.h>
#include <string.h>
#include <pwd.h>

/****** User External Files ********/
#include "extern/introState.ext"

/****** User Table Files ***********/


/****** User Define Files ***********/
#include "define/introState.def"

u64 IntroImageColour;
u64 AlphaChannel;

GSTEXTURE IntroImage;

int introTimer;

u64 oldalpha;


void IntroStart(GSGLOBAL* gsGlobal)
{
	AlphaChannel = 0;
	IntroImageColour = GS_SETREG_RGBAQ(0xFF,0xFF,0xFF,AlphaChannel,0x00);
	oldalpha = gsGlobal->PrimAlpha;
	gsKit_set_primalpha(gsGlobal, GS_SETREG_ALPHA(0,1,0,1,0), 0);
	int i = strlen(relativePath);
	char temp[4096];
	strcpy(temp, relativePath);
	strcat(temp, INTROIMAGEPATH);
	gsKit_texture_png(gsGlobal, &IntroImage, temp);
	
	introTimer = 0;
}

void IntroUpdate(GSGLOBAL* gsGlobal)
{
	introTimer++;

	if(AlphaChannel < 129 && introTimer < 340)
	{
		AlphaChannel++;
	}
	
	if(introTimer > 350 && AlphaChannel > 0)
	{
		AlphaChannel--;
	}
	
	IntroImageColour = GS_SETREG_RGBAQ(AlphaChannel,AlphaChannel,AlphaChannel,AlphaChannel,0x00);
	
	// Switch Game State when timer goes above 500
	if(introTimer > 488)
	{
		introTimer = 0;
		StateMachineChange(&GameMachineState, &MenuState, gsGlobal);
	}
	
	gsKit_clear(gsGlobal, IntroImageColour);
}

void IntroDraw(GSGLOBAL* gsGlobal, u64 colour)
{
	int i = (4096 - gsGlobal->CurrentPointer/1024);
	printf("VRAM: %d\n", i);
	
	gsKit_prim_sprite_texture(gsGlobal, &IntroImage,0,  // X1
						0,  // Y2
						0.0f,  // U1
						0.0f,  // V1
						IntroImage.Width *2, // X2
						IntroImage.Height *2, // X2
						IntroImage.Width *2, // X2
						IntroImage.Height *2, // X2
						2,
						IntroImageColour);
}

void IntroEnd(GSGLOBAL* gsGlobal)
{
	gsKit_set_primalpha(gsGlobal,oldalpha, 0);

	printf("This should Run when MenuState is Gone.\n");
	// Clear VRAM after the Menu is done~
	gsKit_vram_clear(gsGlobal);
}

StateManager IntroState =
{
	IntroStart,
	IntroUpdate,
	IntroDraw,
	IntroEnd
};
