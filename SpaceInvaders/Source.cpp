#include <iostream>
#include <raylib.h>
#include "raymath.h"
#include "AABB.h"
#include <vector>
#include "Actor.h"
#include "Barriers.h"


using namespace std;

//prototype GameOver and GameWin
void GameOver(int scrap);
void GameWin(int scrap);


void main()
{
	/////////////////////////////////////
	////// Initialization
	/////////////////////////////////////

			bool game = true; //set game to true to start gameloop
			int scrap = 0; //scrap is used to represent score and as a currency for upgrades
			int scrapPerKill = 10; //used to set the scrap earnt per kill

			int screenWidth = 1920; // set the width of the screen
			int screenHeight = 1080; // set the height of the screen

			InitAudioDevice();

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
			player.worldPosition.x = (GetScreenWidth() / 2) - (player.texture.width / 2);
			player.worldPosition.y = (GetScreenHeight() - (player.texture.height * 2));
			player.scale = 1;

			Vector2 playerMax; //used to store the lower right position of the player sprite			
			const float PLAYER_SPEED = 200.00; //set the speed of the player
		

		///Initialise Player Shot
			Actor playerShot; //initialise a bullet for the player
			const float SHOT_SPEED = 400; //set the speeds of shots
			bool hasShot = false; //initialise a bool to determin if the player has shot
			bool firstShot = false;
			playerShot.image = LoadImage("laserBlue01.png");
			playerShot.texture = LoadTextureFromImage(playerShot.image);
			playerShot.scale = 0.5;


		//initialise variables used for player uppgrades
			bool shootThrough = false;
			

		////Initialise Enemies
			const float ENEMY_SPEED = 100.00;
			bool goRight = true; //used to reverse the direction of travel

			/////test enemy
			/*Actor enemy;
			enemy.image = LoadImage("enemyBlack2.png");
			enemy.texture = LoadTextureFromImage(enemy.image);
			enemy.scale = 0.5;*/
			//AABB enemyBox; //create an AABB for enemy collisions
			//Vector2 enemyMax; //used to determin lower right corner of enemy sprite

			//create an array of imiges to load for the enemies
			vector<Image> images(5);
			images[0] = LoadImage("enemyBlack1.png");
			images[1] = LoadImage("enemyBlack2.png");
			images[2] = LoadImage("enemyBlack3.png");
			images[3] = LoadImage("enemyBlack4.png");
			images[4] = LoadImage("enemyBlack5.png");

			//variables to create an array of enemies
			Image image1 = LoadImage("enemyBlack2.png");
			Texture texture1 = LoadTextureFromImage(image1);
			const int EN_COLS = 11; //number of collums of enemies
			const int EN_ROWS = 5; //number of rows of enemies
			vector<vector<Actor>> enemies(EN_ROWS,vector<Actor>(EN_COLS));
			int lastX = 0;
			int lastY = 0;

			//Initialise Enemy Shot
			Actor enemyShot;
			const float ENEMY_SHOT_SPEED = 200; //set the speed of enemy shots
			bool EnemyHasShot = false;//set enemy has shot to false to enable them to shoot
			enemyShot.scale = 0.5; // set scale of enemy shot sprite

			int colToShoot; //used to determin wich enemy will shoot
			const int CHANCE_TO_SHOOT = 3; //used to determin frequency of shot

			//create an actor to display explosion
			Actor explode;
			explode.image = LoadImage("laserRed08.png");
			explode.texture = LoadTextureFromImage(explode.image);
			int count = 0;
			const int LENGTH = 15; //amount of time the explode will be displayed		
			enemyShot.image = LoadImage("laserRed07.png");
			enemyShot.texture = LoadTextureFromImage(enemyShot.image);
			explode.scale = 0.5;
			
			
			//////////////////////
			///Set enemies
			//////////////////////
			for (int i = 0; i < EN_ROWS; i++)
			{
				for (int j = 0; j < EN_COLS; j++)
				{
					//create the enemy
					Actor enemy;
					enemy.image = images[i];
					enemy.texture = LoadTextureFromImage(enemy.image);
					enemy.scale = 0.5;

					//set x and y positions for the enemy
					enemy.worldPosition.x = lastX + (texture1.width);
					enemy.worldPosition.y = lastY + (texture1.height);

					lastX = enemy.worldPosition.x;

					//add enemy to array
					enemies[i][j] = enemy;
				}
				lastX = 0;
				lastY = lastY + texture1.height;
			}
			////set x and y positions for the enemy
			//enemy.worldPosition.x = 0 + (enemy.texture.width);
			//enemy.worldPosition.y = 0 + (enemy.texture.height);

	////////////////////////////////////////////////////////////////
	///SOUNDS
	////////////////////////////////////////////////////////////////
			Sound shootfx = LoadSound("sfx_laser2.ogg");
			Sound boom = LoadSound("mixkit-pixel-chiptune-explosion-1692.wav");

    /////////////////////////////////////////////////////////////////
    /// BARRIERS
	//////////////////////////////
			
			const int B_LENGTH = 20; //the length pf each barrier
			const int B_HEIGHT = 10; //the height of each barrier
			const int START_X_1 = (GetScreenWidth() * 0.25f) - ((B_LENGTH * Barriers::SIZE) / 2); //set the starting X point for the first barrier
			const int START_X_2 = (GetScreenWidth() * 0.5f) + ((B_LENGTH * Barriers::SIZE) / 2); //set the starting x point for the second barrier
			const int START_X_3 = (GetScreenWidth() * 0.75f) + ((B_LENGTH * Barriers::SIZE) / 2); //set the starting x point for the third barrier
			const int START_Y = GetScreenHeight() * 0.75f; //set the Y point for all barriers

			int nextX = START_X_1; //used to calculate where to put the next peice on the x
			int nextY = START_Y; //used to calculate where to put the next peice on the y
			vector<vector<Barriers>> barrier1(B_HEIGHT, vector<Barriers>(B_LENGTH));//create a vector to store the first barrier
			vector<vector<Barriers>> barrier2(B_HEIGHT, vector<Barriers>(B_LENGTH));//create a vector to store the second barrier
			vector<vector<Barriers>> barrier3(B_HEIGHT, vector<Barriers>(B_LENGTH));//create a vector to store the third barrier
			////////////////////////////////////////
			////Set up Barrier 1
			////////////////////////////////////////
			for (int i = 0; i < B_HEIGHT; i++)
			{
				for (int j = 0; j < B_LENGTH; j++)
				{
					barrier1[i][j].MakeBarriers(nextX, nextY);
					nextX -= Barriers::SIZE;
				}
				nextX = START_X_1;
				nextY -= Barriers::SIZE;
			}

			///////////////////////////////////////////
			///////Set Up Barrier 2
			///////////////////////////////////////////
			nextX = START_X_2;
			nextY = START_Y;
			for (int i = 0; i < B_HEIGHT; i++)
			{
				for (int j = 0; j < B_LENGTH; j++)
				{
					barrier2[i][j].MakeBarriers(nextX, nextY);
					nextX -= Barriers::SIZE;
				}
				nextX = START_X_2;
				nextY -= Barriers::SIZE;
			}

			/////////////////////////////////////////////
			//////Set Up Barrier 3
			/////////////////////////////////////////////
			nextX = START_X_3;
			nextY = START_Y;
			for (int i = 0; i < B_HEIGHT; i++)
			{
				for (int j = 0; j < B_LENGTH; j++)
				{
					barrier3[i][j].MakeBarriers(nextX, nextY);
					nextX += Barriers::SIZE;
				}
				nextX = START_X_3;
				nextY -= Barriers::SIZE;
			}
	

	//////////////////////////////////////////////
	////// Game Loop
	//////////////////////////////////////////////
	while (game)
	{
		///////////////////////////////////////////////////////
		/////UPDATE
		//////////////////////////////////////////////////////

		if (player.isDead)
		{
			game = false;
			GameOver(scrap);
		}
		//calculate delta time
		previousTime = currentTime;
		currentTime = GetTime();
		deltaTime = (currentTime - previousTime);

		
		//calculate the new x and y position of the lower right corner of player sprite
		playerMax.x = player.worldPosition.x + (player.texture.width * player.scale);
		playerMax.y = player.worldPosition.y + (player.texture.height * player.scale);

		////calculate the new x and y positios of the lower right corner of the enemy
		//enemy.Max.x = enemy.worldPosition.x + (enemy.texture.width * enemy.scale);
		//enemy.Max.y = enemy.worldPosition.y + (enemy.texture.height * enemy.scale);

		for (int i = 0; i < EN_ROWS; i++)
		{
			for (int j = 0; j < EN_COLS; j++)
			{
				//calculate the new x and y positios of the lower right corner of the enemy
				enemies[i][j].Max.x = enemies[i][j].worldPosition.x + (enemies[i][j].texture.width * enemies[i][j].scale);
				enemies[i][j].Max.y = enemies[i][j].worldPosition.y + (enemies[i][j].texture.height * enemies[i][j].scale);

				//Fit enemy AABB
				enemies[i][j].Box.Fit(enemies[i][j].worldPosition, enemies[i][j].Max);
			}
		}

		//refit AABBs
		player.Box.Fit(player.worldPosition, playerMax);
		//enemy.Box.Fit(enemy.worldPosition, enemy.Max);

		////////////////////////////////////////
		////Trigger Enemy Shot
		////////////////////////////////////////
		if (firstShot)
		{
			if (!EnemyHasShot)
			{
				//seed ramd
				srand((unsigned)time(NULL));
				int willShoot = rand() % CHANCE_TO_SHOOT;

				if (willShoot == (CHANCE_TO_SHOOT -1))
				{
					colToShoot = rand() % EN_COLS;
					for (int i = 0; i < EN_ROWS; i++)
					{
						if (!enemies[i][colToShoot].isDead)
						{
							enemyShot.worldPosition.y = enemies[i][colToShoot].worldPosition.y + enemies[i][colToShoot].texture.height;
							enemyShot.worldPosition.x = enemies[i][colToShoot].worldPosition.x - (enemies[i][colToShoot].texture.width / 2);;
							EnemyHasShot = true;
						}
					}
				}
			}
			for (int i = 0; i < EN_ROWS; i++)
			{
				for (int j = 0; j < EN_COLS; j++)
				{
					if (enemies[i][j].worldPosition.y + (enemies[i][j].texture.height * enemies[i][j].scale) >= player.worldPosition.y)
					{
						game = false;
						GameOver(scrap);
					}
				}
			}
		}

		if (!firstShot)
		{
			for (int i = 0; i < EN_ROWS; i++)
			{
				for (int j = 0; j < EN_COLS; j++)
				{
					if (enemies[i][j].worldPosition.y + (enemies[i][j].texture.height * enemies[i][j].scale) >= player.worldPosition.y)
					{
						scrap += 99999;
						game = false;
						GameWin(scrap);
					}
				}
			}
		}

		/////////////////////////////////////////////////
		///// Draw
		////////////////////////////////////////////////
		BeginDrawing();

		ClearBackground(BLACK);


		///////////////////////////////////////////
		//////Draw Barriers
		///////////////////////////////////////////
		////draw Barrier 1
		for (int i = 0; i < B_HEIGHT; i++)
		{
			for (int j = 0; j < B_LENGTH; j++)
			{
				if (!barrier1[i][j].isDead)
				{
					barrier1[i][j].DrawBarriers();
				}
			}
		}
		////draw barrier 2
		for (int i = 0; i < B_HEIGHT; i++)
		{
			for (int j = 0; j < B_LENGTH; j++)
			{
				if (!barrier2[i][j].isDead)
				{
					barrier2[i][j].DrawBarriers();
				}
			}
		}
		///draw barrier 3
		for (int i = 0; i < B_HEIGHT; i++)
		{
			for (int j = 0; j < B_LENGTH; j++)
			{
				if (!barrier3[i][j].isDead)
				{
					barrier3[i][j].DrawBarriers();
				}
			}
		}

		if (!player.isDead)
		{
			//player.Box.DebugBox(RED);
			player.Draw();
		}		
		
		if (hasShot) //check if the player has shot
		{
			/////////////////////
			//Move the shot
			/////////////////////
			playerShot.worldPosition.y -= (deltaTime * SHOT_SPEED); //continue to move the shot up the screen every frame
			playerShot.Draw();

			////////////////////////////////////
			/////detect when shot leeves screen
			////////////////////////////////////
			if (playerShot.worldPosition.y <= 0) //if the players shot goes past the top of the screen
			{
				hasShot = false; //set has shot to false
			}

			/*if (enemy.Box.Overlaps(playerShot.worldPosition))
			{
				enemy.isDead = true;
			}*/

			////////////////////////////////////
			/////Collisions for Enemies
			////////////////////////////////////
			for (int i = 0; i < EN_ROWS; i++)
			{
				for (int j = 0; j < EN_COLS; j++)
				{
					if (enemies[i][j].Box.Overlaps(playerShot.worldPosition))
					{
						explode.worldPosition.x = enemies[i][j].worldPosition.x + (enemies[i][j].texture.width * enemies[i][j].scale) / 2;
						explode.worldPosition.y = enemies[i][j].worldPosition.y + (enemies[i][j].texture.height * enemies[i][j].scale) / 2;
						explode.isDead = false;
						enemies[i][j].isDead = true;
						enemies[i][j].worldPosition.x = -100;
						PlaySound(boom);
						scrap += scrapPerKill;
						if (!shootThrough)
						{
							hasShot = false;
						}
					}
				}
			}

			/////////////////////////////////////////
			//////////Collisions For Barriers
			/////////////////////////////////////////
			//Barrier 1 collisions
			for (int i = 0; i < B_HEIGHT; i++)
			{
				for (int j = 0; j < B_LENGTH; j++)
				{
					if (!barrier1[i][j].isDead)
					{
						if (barrier1[i][j].Box.Overlaps(playerShot.worldPosition))
						{
							PlaySound(boom);
							barrier1[i][j].isDead = true;
							barrier1[i][j].worldPosition.x = -100;
							if (!shootThrough)
							{
								hasShot = false;
							}
						}
					}
				}
			}
			//barrier 2 collision
			for (int i = 0; i < B_HEIGHT; i++)
			{
				for (int j = 0; j < B_LENGTH; j++)
				{
					if (!barrier2[i][j].isDead)
					{
						if (barrier2[i][j].Box.Overlaps(playerShot.worldPosition))
						{
							PlaySound(boom);
							barrier2[i][j].isDead = true;
							barrier2[i][j].worldPosition.x = -100;
							if (!shootThrough)
							{
								hasShot = false;
							}
						}
					}
				}
			}
			//barreir 3 collision
			for (int i = 0; i < B_HEIGHT; i++)
			{
				for (int j = 0; j < B_LENGTH; j++)
				{
					if (!barrier3[i][j].isDead)
					{
						if (barrier3[i][j].Box.Overlaps(playerShot.worldPosition))
						{
							PlaySound(boom);
							barrier3[i][j].isDead = true;
							barrier3[i][j].worldPosition.x = -100;
							if (!shootThrough)
							{
								hasShot = false;
							}
						}
					}
				}
			}
		}

		if (EnemyHasShot)
		{
			enemyShot.worldPosition.y += (deltaTime * ENEMY_SHOT_SPEED);
			if (enemyShot.worldPosition.y >= GetScreenHeight())
			{
				EnemyHasShot = false;
			}
			enemyShot.Draw();

			///////////////////////////////////////////////////
			//////Collision for player
			///////////////////////////////////////////////////
			if (player.Box.Overlaps(enemyShot.worldPosition)) //check if player gets hit
			{
				PlaySound(boom); //play sound
				player.isDead = true; //set player to dead
				player.worldPosition.x = -100; //moove player off screen
				EnemyHasShot = false;
			}

			/////////////////////////////////////////
			//////////Collisions For Barriers
			/////////////////////////////////////////
			//Barrier 1 collisions
			for (int i = 0; i < B_HEIGHT; i++)
			{
				for (int j = 0; j < B_LENGTH; j++)
				{
					if (!barrier1[i][j].isDead)
					{
						if (barrier1[i][j].Box.Overlaps(enemyShot.worldPosition))
						{
							PlaySound(boom);
							barrier1[i][j].isDead = true;
							barrier1[i][j].worldPosition.x = -100;
							if (!shootThrough)
							{
								EnemyHasShot = false;
							}
						}
					}
				}
			}
			//barrier 2 collision
			for (int i = 0; i < B_HEIGHT; i++)
			{
				for (int j = 0; j < B_LENGTH; j++)
				{
					if (!barrier2[i][j].isDead)
					{
						if (barrier2[i][j].Box.Overlaps(enemyShot.worldPosition))
						{
							PlaySound(boom);
							barrier2[i][j].isDead = true;
							barrier2[i][j].worldPosition.x = -100;
							if (!shootThrough)
							{
								EnemyHasShot = false;
							}
						}
					}
				}
			}
			//barreir 3 collision
			for (int i = 0; i < B_HEIGHT; i++)
			{
				for (int j = 0; j < B_LENGTH; j++)
				{
					if (!barrier3[i][j].isDead)
					{
						if (barrier3[i][j].Box.Overlaps(enemyShot.worldPosition))
						{
							PlaySound(boom);
							barrier3[i][j].isDead = true;
							barrier3[i][j].worldPosition.x = -100;
							if (!shootThrough)
							{
								EnemyHasShot = false;
							}
						}
					}
				}
			}
		
		}

		////////////////////////////
		//////Enemy Movement
		////////////////////////////
		for (int i = 0; i < EN_ROWS; i++)
		{
			for (int j = 0; j < EN_COLS; j++)
			{
				if (!enemies[i][j].isDead)
				{
					if (goRight)
					{
						enemies[i][j].worldPosition.x += (deltaTime * ENEMY_SPEED);

						if (enemies[i][j].worldPosition.x + (enemies[i][j].texture.width / 2) >= GetScreenWidth())
						{
							for (int i = 0; i < EN_ROWS; i++)
							{
								for (int j = 0; j < EN_COLS; j++)
								{
									enemies[i][j].worldPosition.y += ((deltaTime * ENEMY_SPEED) * 10);
									goRight = false;
								}
							}
						}
					}
					if (!goRight)
					{
						enemies[i][j].worldPosition.x -= (deltaTime * ENEMY_SPEED);
						if (enemies[i][j].worldPosition.x <= 0)
						{
							for (int i = 0; i < EN_ROWS; i++)
							{
								for (int j = 0; j < EN_COLS; j++)
								{
									enemies[i][j].worldPosition.y += ((deltaTime * ENEMY_SPEED) * 10);
									goRight = true;
								}
							}
						}
					}

					//enemies[i][j].Box.DebugBox(GREEN);
					enemies[i][j].Draw();
				}
			}
		}

		
		if (!explode.isDead)
		{
			count ++; //calculate the amount of time the explosion has been on screen
			if (count > LENGTH) //if the explosion has been on for longert then LENGTH
			{
				explode.isDead = true; //set is dead to true so explosion will stop drawing
				count = 0; // reset counter
			}
			explode.Draw();
		}

		//if (!enemy.isDead)
		//{
		//	if (goRight)
		//	{
		//		enemy.worldPosition.x += (deltaTime * ENEMY_SPEED);

		//		if (enemy.worldPosition.x + (enemy.texture.width / 2) >= GetScreenWidth())
		//		{
		//			enemy.worldPosition.y += ((deltaTime * ENEMY_SPEED) * 2);
		//			goRight = false;
		//		}
		//	}
		//	if (!goRight)
		//	{
		//		enemy.worldPosition.x -= (deltaTime * ENEMY_SPEED);
		//		if (enemy.worldPosition.x <= 0)
		//		{
		//			enemy.worldPosition.y += ((deltaTime * ENEMY_SPEED) * 2);
		//			goRight = true;
		//		}
		//	}

		//	enemy.Box.DebugBox(GREEN);
		//	enemy.Draw();
		//}	


		DrawText(TextFormat("Scrap: %05i", scrap), 10, 10, 20, WHITE);
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
				firstShot = true;
				playerShot.worldPosition.x = player.worldPosition.x + (player.texture.width / 2) - (playerShot.texture.width / 2);
				playerShot.worldPosition.y = player.worldPosition.y - (playerShot.texture.height / 2);
				PlaySound(shootfx);
			}
			
		}

		if (IsKeyPressed(KEY_ESCAPE))///if escape key is pressed
		{
			CloseWindow(); //close the game window
			game = false; //end the loop
		}
		
	}
	
}

void GameOver(int scrap)
{
	bool esc = false;

	while (!esc)
	{
		BeginDrawing();

		ClearBackground(BLACK);


		DrawText(TextFormat("Scrap: %05i", scrap), (GetScreenWidth() / 2) - 50, 10, 20, WHITE);
		DrawText("GAME OVER", (GetScreenWidth() / 2) - (MeasureText("GAME OVER", 50) / 2), GetScreenHeight() / 2, 50, WHITE);

		if (IsKeyPressed(KEY_ESCAPE))///if escape key is pressed
		{
			CloseWindow(); //close the game window
			esc = true; //end the loop
		}

		EndDrawing();
	}
}

void GameWin(int scrap)
{
	bool esc = false;

	while (!esc)
	{
		BeginDrawing();

		ClearBackground(BLACK);
		string message = "The alien ships rach you and you discuss tearms of a treaty. \n They offer to work with you to better the human race. \n They also bring you cake.";

		DrawText(TextFormat("Scrap: %05i", scrap), (GetScreenWidth() / 2) - 50, 10, 20, WHITE);
		DrawText(message.c_str(), (GetScreenWidth() / 2) - (MeasureText(message.c_str(), 50) / 2), GetScreenHeight() / 2, 50, WHITE);

		if (IsKeyPressed(KEY_ESCAPE))///if escape key is pressed
		{
			CloseWindow(); //close the game window
			esc = true; //end the loop
		}

		EndDrawing();
	}
}