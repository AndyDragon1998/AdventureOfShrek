#ifndef STARFIELD
#define STARFIELD

#include <gsKit.h>
#include <dmaKit.h>
#include <gsToolkit.h>

typedef struct
{
	GSTEXTURE texture;
	u64 TexCol;
	float posX;
	float posY;
	float pos2X;
	float pos2Y;
	float pos3X;
	float pos3Y;
} Starfield;

void InitializeStarfield(GSGLOBAL* gsGlobal);

void ScrollBackground(GSGLOBAL* gsGlobal);

void DrawBackground(GSGLOBAL* gsGlobal, u64 colour);

#endif
