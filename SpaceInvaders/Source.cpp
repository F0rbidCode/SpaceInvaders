#include <iostream>
#include <raylib.h>
#include "raymath.h"


using namespace std;

struct Actor {
	Vector2 worldPosition;
	Texture2D texture;
	Image image;

public:
	void Draw()
	{
		DrawTextureEx(texture, worldPosition, 0, 1, WHITE);
	}
};

void main()
{
	/////////////////////////////////////
	////// Initialization
	/////////////////////////////////////
	bool game = true; //set game to true to start gameloop
	
	int screenWidth = 1920; // set the width of the screen
	int screenHeight = 1080; // set the height of the screen

	SetTargetFPS(60);

	//initialise variables used to calculate delta time
	float currentTime = GetTime();
	float previousTime;
	float deltaTime;
	
	InitWindow(screenWidth, screenHeight, "Space Invaders");

	Actor player; //initialise player 
	player.image = LoadImage("playerShip2_blue.png");
	player.texture = LoadTextureFromImage(player.image);
	//set x y and z positions of the player
	player.worldPosition.x = (GetScreenWidth() / 2);
	player.worldPosition.y = (GetScreenHeight() - (player.texture.height * 2));

	const float PLAYER_SPEED = 100.00; //set the speed of the player

	
	///////////////////////////////////////////////
	////// Game Loop
	//////////////////////////////////////////////
	while (game)
	{
		///////////////////////////////////////////////////////
		/////UPDATE
		//////////////////////////////////////////////////////
		
		//calculate delta time
		previousTime = currentTime;
		currentTime = GetTime();
		deltaTime = (currentTime - previousTime);


		/////////////////////////////////////////////////
		///// Draw
		////////////////////////////////////////////////
		BeginDrawing();

		ClearBackground(BLACK);

		player.Draw();
		
		EndDrawing();
		
		/////////////////////////////////////////////////////
		///Get User Inpiut
		/////////////////////////////////////////////////////

		if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
		{
			player.worldPosition.x += (deltaTime * PLAYER_SPEED);
		}
		if (IsKeyDown (KEY_LEFT) || IsKeyDown(KEY_A))
		{
			player.worldPosition.x -= (deltaTime * PLAYER_SPEED);
		}


		if (IsKeyPressed(KEY_ESCAPE))///if escape key is pressed
		{
			CloseWindow(); //close the game window
			game = false; //end the loop
		}
		
	}
	//system("pause");
}