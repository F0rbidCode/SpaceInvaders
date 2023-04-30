#include <iostream>
#include <raylib.h>
#include "raymath.h"
#include "AABB.h"


using namespace std;

struct Actor {
	Vector2 worldPosition; //used to store world position
	Texture2D texture; // used to store texture
	Image image; // used to store image
	bool isDead = false; //used to determin if actor is still alive
	float scale; //used to set the scale of the actor

public:
	void Draw()
	{
		DrawTextureEx(texture, worldPosition, 0, scale, WHITE);
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

		///Initialise player
			Actor player; //initialise player 
			player.image = LoadImage("playerShip2_blue.png");
			player.texture = LoadTextureFromImage(player.image);
			//set x and y positions of the player
			player.worldPosition.x = (GetScreenWidth() / 2);
			player.worldPosition.y = (GetScreenHeight() - (player.texture.height * 2));
			player.scale = 1;

			AABB playerBox;
			Vector2 playerMax; //used to store the lower right position of the player sprite			
			const float PLAYER_SPEED = 200.00; //set the speed of the player
		

		///Initialise Player SHot
			Actor playerShot; //initialise a bullet for the player
			const float SHOT_SPEED = 400; //set the speeds of shots
			bool hasShot = false; //initialise a bool to determin if the player has shot
			playerShot.image = LoadImage("laserBlue01.png");
			playerShot.texture = LoadTextureFromImage(playerShot.image);
			playerShot.scale = 0.5;
			

		////Initialise Enemies
			const float ENEMY_SPEED = 100.00;
			bool goRight = true; //used to reverse the direction of travel

			Actor enemy;
			enemy.image = LoadImage("enemyBlack2.png");
			enemy.texture = LoadTextureFromImage(enemy.image);
			enemy.scale = 0.5;
			AABB enemyBox; //create an AABB for enemy collisions
			Vector2 enemyMax; //used to determin lower right corner of enemy sprite
			

			//set x and y positions for the enemy
			enemy.worldPosition.x = 0 + (enemy.texture.width);
			enemy.worldPosition.y = 0 + (enemy.texture.height);

	
	
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

		
		//calculate the new x and y position of the lower right corner of player sprite
		playerMax.x = player.worldPosition.x + (player.texture.width * player.scale);
		playerMax.y = player.worldPosition.y + (player.texture.height * player.scale);

		//calculate the new x and y positios of the lower right corner of the enemy
		enemyMax.x = enemy.worldPosition.x + (enemy.texture.width * enemy.scale);
		enemyMax.y = enemy.worldPosition.y + (enemy.texture.height * enemy.scale);

		//refit AABBs
		playerBox.Fit(player.worldPosition, playerMax);
		enemyBox.Fit(enemy.worldPosition, enemyMax);


		/////////////////////////////////////////////////
		///// Draw
		////////////////////////////////////////////////
		BeginDrawing();

		ClearBackground(BLACK);

		if (!player.isDead)
		{
			playerBox.DebugBox(RED);
			player.Draw();
		}		
		
		if (hasShot) //check if the player has shot
		{
			playerShot.worldPosition.y -= (deltaTime * SHOT_SPEED); //continue to move the shot up the screen every frame
			playerShot.Draw();

			if (playerShot.worldPosition.y <= 0) //if the players shot goes past the top of the screen
			{
				hasShot = false; //set has shot to false
			}

			if (enemyBox.Overlaps(playerShot.worldPosition))
			{
				enemy.isDead = true;
			}
		}

		if (!enemy.isDead)
		{
			if (goRight)
			{
				enemy.worldPosition.x += (deltaTime * ENEMY_SPEED);

				if (enemy.worldPosition.x + (enemy.texture.width / 2) >= GetScreenWidth())
				{
					enemy.worldPosition.y += ((deltaTime * ENEMY_SPEED) * 2);
					goRight = false;
				}
			}
			if (!goRight)
			{
				enemy.worldPosition.x -= (deltaTime * ENEMY_SPEED);
				if (enemy.worldPosition.x <= 0)
				{
					enemy.worldPosition.y += ((deltaTime * ENEMY_SPEED) * 2);
					goRight = true;
				}
			}

			enemyBox.DebugBox(GREEN);
			enemy.Draw();
		}
		
		EndDrawing();
		
		/////////////////////////////////////////////////////
		///Get User Inpiut
		/////////////////////////////////////////////////////

		if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
		{			
			if (player.worldPosition.x + player.texture.width >= GetScreenWidth())
			{

			}
			else
			{
				player.worldPosition.x += (deltaTime * PLAYER_SPEED);
			}
		}
		if (IsKeyDown (KEY_LEFT) || IsKeyDown(KEY_A))
		{
			if (player.worldPosition.x <= 0)
			{

			}
			else
			{
				player.worldPosition.x -= (deltaTime * PLAYER_SPEED);
			}
			
		}

		if (IsKeyDown(KEY_SPACE))
		{
			if (!hasShot)
			{
				hasShot = true;
				playerShot.worldPosition.x = player.worldPosition.x + (player.texture.width / 2) - (playerShot.texture.width / 2);
				playerShot.worldPosition.y = player.worldPosition.y - (playerShot.texture.height / 2);
			}
			
		}

		if (IsKeyPressed(KEY_ESCAPE))///if escape key is pressed
		{
			CloseWindow(); //close the game window
			game = false; //end the loop
		}
		
	}
	//system("pause");
}