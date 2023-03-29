/*****************************************************************************/
/*  Автор         		: Дракончик				                             */
/*  Приставка     		: Sony PlayStation 2					             */
/*  ОС			  		: OSDSYS											 */
/*  Язык	      		: GNU C                                              */
/*                                                                           */
/*  Содержимое файла	: Стадия игровой детельности                         */
/*  Атрибут файла		: SOURCE                                             */
/*  Имя файла     		: gameState.c	                                     */
/*                                                                           */
/*****************************************************************************/

#include "include/gameState.h"
#include "include/menuState.h"
#include "include/overState.h"
#include "include/introState.h"
#include "include/pad.h"
#include "include/musicManager.h"
#include <gsKit.h>

#include "include/background.h"
#include <stdio.h>
#include <stdlib.h>

extern StateMachine GameMachineState;

Starfield stars;

/****** User External Files ********/
#include "extern/gameState.ext"

/****** User Table Files ***********/


/****** User Define Files ***********/
#include "define/gameState.def"

// Graphics Synthesizer Texture structs
GSTEXTURE BackgroundImage;
GSTEXTURE Player;
GSTEXTURE Fire;

// GSPixel sprite[32*32];
// GSIMAGE Player;

float bgx = 0.0f;
float bgy = 0.0f;
float bgx2;
float bgy2 = 0.0f;

const float speed = 3.5f;

bool onGround = true;

bool GameOverBoolean;

typedef struct {
  int X;
  int Y;
  int width;
  int height;
} Obstacle;

BGM BackgroundMusic;

void InitializeStarfield(GSGLOBAL* gsGlobal)
{

	char tempy[4096];
	strcpy(tempy, relativePath);
	strcat(tempy, "/Graphics/Background/background_image.png");
	gsKit_texture_png(gsGlobal, &BackgroundImage,tempy);
	
	char temp[4096];
	strcpy(temp, relativePath);
	strcat(temp, "/Audio/BGM/Somebody.wav");
	BackgroundMusic.fileName = temp;

	char tem[4096];
	strcpy(tem, relativePath);	
	strcat(tem, OBSTACLE);
	gsKit_texture_png(gsGlobal, &Fire,tem);

	

	
	LoadMusic(&BackgroundMusic);
	
	bgx = 0.0f;
	bgy = 0.0f;
	bgx2 = 900.0f;
	bgy2 = 0.0f;
	stars.TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);

}

void ScrollBackground(GSGLOBAL *gsGlobal)
{
	bgx-= speed;	
	bgx2-= speed;	
	//printf("Y1 = %.2f Y2 = %.2f\n", stars.posY, stars.pos2Y);	
	// If Stars image reaches end of screen.	
	if(bgx < -900.0f)
	{
		bgx = 900.0f;
	}
	
	if(bgx2 < -900.0f)
	{
		bgx2 = 900.0f;
	}
	
	printf("X1 = %f\n",bgx);
	printf("X2 = %f\n",bgx2);
}


// Player variables
int playerX = 100;
int playerY = 450;
int playerVelY = 0;
int playerJumpForce = 20;
bool player_alive = true;

int score = 0;
int record = 0;

Obstacle fireArray[10];


/*
void make_obstacle(GSGLOBAL* gsGlobal, u64 colour) {

	gsKit_prim_sprite_texture(gsGlobal, &Fire, 0,  // X1
						0,  // Y1
						0,  // U1
						0.0f,  // V1
						Fire.Width, // X2
						Fire.Height, // Y2
						Fire.Width, // U2
						Fire.Height, // V2
						2,
						colour);

	int firex = 300;
	int firey = 380;						
	gsKit_prim_sprite_texture(gsGlobal, &Fire, firex,  // X1
						firey,  // Y1
						0,  // U1
						0.0f,  // V1
						Fire.Width + firex, // X2
						Fire.Height + firey, // Y2
						Fire.Width, // U2
						Fire.Height, // V2
						2,
						colour);

}
*/
// Gravity constant
const int GRAVITY = 1;


int firex = 300;
int firey = 480;

int currentFire = 0;

float displace = speed;

int timer = 0;
int rate = 80;

void spawnFire(float posX) {
	float posY = 430;
	fireArray[currentFire].X = posX;
	fireArray[currentFire].Y = posY;
	currentFire++;
}

void updateFire(GSGLOBAL* gsGlobal) {

	hitCheck(gsGlobal);
	
	if(!player_alive) {
		GameOverBoolean = true;
	}

	int x = rand()%100;
	if (timer > rate) { 
		spawnFire((float)x+600);
		timer = 0;
	}
	timer++;
	for(int i = 0; i < currentFire; i++) {
	
		fireArray[i].X -= speed;
		
	}
}

void drawFire(GSGLOBAL* gsGlobal, u64 colour) {
	for(int i = 0; i < currentFire; i++) {
	
		gsKit_prim_sprite_texture(gsGlobal, &Fire, fireArray[i].X,  // X1
							fireArray[i].Y,  // Y1
							0,  // U1
							0.0f,  // V1
							Fire.Width + fireArray[i].X, // X2
							Fire.Height + fireArray[i].Y, // Y2
							Fire.Width, // U2
							Fire.Height, // V2
							2,
							colour);
	}
}


void hitCheck(GSGLOBAL* gsGlobal) {
	for(int i = 0; i < currentFire; i++) {
		if((playerX+50) + (Player.Width / 2)-50 > fireArray[i].X &&
		 (playerX+50) < fireArray[i].X + Fire.Width) {
			if((playerY+50) + (Player.Height / 2)-50 > fireArray[i].Y &&
			 (playerY+50) < fireArray[i].Y + Fire.Height) {
				player_alive = false;
				PlaystationGamePad.HAPTIC_MOTOR = true;
			
			} else {
			PlaystationGamePad.HAPTIC_MOTOR = false;
		}
	}
}
}
/*
void place_obstacles(GSGLOBAL* gsGlobal, u64 colour, int num_obstacles, float displace) {
  for (int i = 0; i < num_obstacles; i++) {
  
    obstacles[i].x = playerX + 500;
    obstacles[i].y = 380;
    obstacles[i].width = Fire.Width;
    obstacles[i].height = Fire.Height;
   
   	int firex = 300 * i - displace;
	int firey = 380;
	gsKit_prim_sprite_texture(gsGlobal, &Fire, firex,  // X1
							firey,  // Y1
							0,  // U1
							0.0f,  // V1
							Fire.Width + firex, // X2
							Fire.Height + firey, // Y2
							Fire.Width, // U2
							Fire.Height, // V2
							2,
							colour);
  }
  
}
*/

// Update function for player movement
void UpdatePlayer()
{

	// Apply gravity to player velocity
	playerVelY += GRAVITY;
	// Update player position based on velocity
	playerY += playerVelY;

	// Check if player is on the ground (i.e. at bottom of screen)
	if (playerY >= 400)
	{
	    // Set player position to ground level
	    playerY = 400;
	    // Reset player velocity
	    playerVelY = 0;
	}
	if (playerY != 400) {
		onGround = false;
	} else {
		onGround = true;
	}
	
	// Check if player wants to jump
	if (PlaystationGamePad.BUTTON_X_KEY_TAP && onGround)
	{
	    // Set player velocity to jump force
	    playerVelY = -playerJumpForce;
	}

}

// Draw function for player sprite
void DrawPlayer(GSGLOBAL* gsGlobal, u64 colour)
{


	// gsKit_texture_image(gsGlobal, &Player, 0, 0, tempy);
	
	// Draw player sprite at current position
	// gsKit_prim_sprite(gsGlobal, playerX, playerY, 32, 32, 2, colour);
	// gsKit_prim_sprite(gsGlobal, x, y, 0, 0, width, height, 2, colour);

}


void GameStart(GSGLOBAL* gsGlobal)
{
	char tempy[4096];
	strcpy(tempy, relativePath);
	strcat(tempy, PLAYER);
	gsKit_texture_png(gsGlobal, &Player,tempy);
	

	
	SetUpFont(gsGlobal);
	InitializeStarfield(gsGlobal);
	GameOverBoolean = false;

}

void GameUpdate(GSGLOBAL* gsGlobal)
{	
	PlayMusic(&BackgroundMusic);

	UpdatePlayer();
	updateFire(gsGlobal);
	
	if(GameOverBoolean) {
		StateMachineChange(&GameMachineState, &OverState, gsGlobal);
	}
	
	if(PlaystationGamePad.UP_KEY_TAP)
	{

	}
	
	if(PlaystationGamePad.DOWN_KEY_TAP)
	{

	}
	
	if(PlaystationGamePad.LEFT_KEY_TAP)
	{

	}	
	
	if(PlaystationGamePad.RIGHT_KEY_TAP)
	{

	}
	
	if(PlaystationGamePad.BUTTON_X_KEY_TAP)
	{

	}
	
	if(PlaystationGamePad.BUTTON_O_KEY_TAP)
	{

	}
	
}

void ScoreDraw(GSGLOBAL* gsGlobal, u64 colour)
{

	
	score += 1;

}

void GameDraw(GSGLOBAL* gsGlobal, u64 colour)
{
	//float displace = 3.0f;
	//displace += 3.0f;

	ScrollBackground(gsGlobal);


	
	gsKit_prim_sprite_texture(gsGlobal, &BackgroundImage, bgx,  // X1
								bgy,  // Y1
								0,  // U1
								0.0f,  // V1
								BackgroundImage.Width  * 2 + bgx, // X2
								BackgroundImage.Height * 2, // Y2
								BackgroundImage.Width  * 2, // U2
								BackgroundImage.Height * 2, // V2
								2,
								colour);
										
	gsKit_prim_sprite_texture(gsGlobal, &BackgroundImage, bgx2,  // X1
								bgy2,  // Y1
								0,  // U1
								0.0f,  // V1
								BackgroundImage.Width  * 2 + bgx2, // X2
								BackgroundImage.Height * 2, // Y2
								BackgroundImage.Width  * 2, // U2
								BackgroundImage.Height * 2, // V2
								2,
								colour);

/*
	gsKit_prim_sprite_texture(gsGlobal, &BackgroundImage, bgx2 + BackgroundImage.Width * 2,  // X1
								bgy2,  // Y1
								1.0f,  // U1
								0.0f,  // V1
								bgx2, // X2
								BackgroundImage.Height * 2, // Y2
								0.0f, // U2
								BackgroundImage.Height * 2, // V2
								2,
								colour);
*/                                
	DrawText(200, 10, "CX~N",3, gsGlobal, &colour);
  	char str[10];

  	sprintf(str, "%d", score);
  	DrawText(340, 10, str,3, gsGlobal, &colour);
  	score++;
	//make_obstacle(gsGlobal, &Fire);
	// Draw player sprite
	// DrawPlayer(gsGlobal, colour);
	
	drawFire(gsGlobal, colour);
	
	gsKit_prim_sprite_texture(gsGlobal, &Player, playerX,  // X1
					playerY,  // Y1
					0.0f,  // U1
					0.0f,  // V1
					playerX + Player.Width / 2, // X2
					playerY + Player.Height / 2, // Y2
					Player.Width, // U2
					Player.Height, // V2
					2,
					colour);

	
	// place_obstacles(gsGlobal, colour, 10, displace);
/*					
	gsKit_prim_sprite_texture(gsGlobal, &Fire, firex,  // X1
						firey,  // Y1
						0,  // U1
						0.0f,  // V1
						Fire.Width + firex, // X2
						Fire.Height + firey, // Y2
						Fire.Width, // U2
						Fire.Height, // V2
						2,
						colour);

*/										
	//DrawText(120, 380, "BLB KTCJV",2, gsGlobal, &colour);	
}

void GameEnd(GSGLOBAL* gsGlobal)
{
	gsKit_vram_clear(gsGlobal);
}

StateManager GameState =
{
	GameStart,
	GameUpdate,
	GameDraw,
	GameEnd
};
