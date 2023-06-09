#include <iostream>
#include <raylib.h>
#include "raymath.h"
#include "AABB.h"
#include <vector>
#include "Actor.h"
#include "Barriers.h"


using namespace std;

//prototype Setup GameOver and GameWin
void Reset(vector<vector<Actor>> &enemies, vector<vector<Barriers>> &barrier1, vector<vector<Barriers>> &barrier2, vector<vector<Barriers>> &barrier3);

void GameOver(int scrap, bool &game);
void GameWin(int scrap, bool &game);

void Upgrade(int &scrap, bool &shootThrough, bool &trippleShot, float &playerSpeedBoost, float &shootSpeedBoost);



static const int EN_COLS = 11; //number of collums of enemies
static const int EN_ROWS = 5; //number of rows of enemies
static const int B_LENGTH = 20; //the length pf each barrier
static const int B_HEIGHT = 10; //the height of each barrier

void main()
{
	/////////////////////////////////////
	////// Initialization
	/////////////////////////////////////

			bool game = true; //set game to true to start gameloop
			bool paused = false;
			int scrap = 0; //scrap is used to represent score and as a currency for upgrades
			int scrapPerKill = 10; //used to set the scrap earnt per kill
			int kills = 0; //stores the number of kills to determin when level has been cleared

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
			player.image = LoadImage(".\\data\\playerShip2_blue.png");
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
			
			playerShot.isDead = true;
			bool firstShot = false;
			playerShot.image = LoadImage(".\\data\\laserBlue01.png");
			playerShot.texture = LoadTextureFromImage(playerShot.image);
			playerShot.scale = 0.5;


		//initialise variables used for player uppgrades
			bool shootThrough = false; //enables the main bullet to continue after hitting an object
			bool trippleShot = false; //adds 2 aditional bullets
			float playerSpeedBoost = 1.0; //used to increase the players speed
			float shootSpeedBoost = 1.0; //used to increase the players shoot speed

			pair<Actor, Actor> extraShots; //create a pair to hold the extra shots from upgrade

			//extraShots.first = playerShot;			
			extraShots.first = playerShot;
			extraShots.second = playerShot;
			

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
			images[0] = LoadImage(".\\data\\enemyBlack1.png");
			images[1] = LoadImage(".\\data\\enemyBlack2.png");
			images[2] = LoadImage(".\\data\\enemyBlack3.png");
			images[3] = LoadImage(".\\data\\enemyBlack4.png");
			images[4] = LoadImage(".\\data\\enemyBlack5.png");

			//variables to create an array of enemies
			Image image1 = LoadImage(".\\data\\enemyBlack2.png");
			Texture texture1 = LoadTextureFromImage(image1);

			
			vector<vector<Actor>> enemies(EN_ROWS,vector<Actor>(EN_COLS));
			int lastX = 0;
			int lastY = 0;

			//Initialise Enemy Shot
			Actor enemyShot;
			const float ENEMY_SHOT_SPEED = 200; //set the speed of enemy shots
			float enemySpeedUp = 1;
			float levelUp = 0;
			bool EnemyHasShot = false;//set enemy has shot to false to enable them to shoot
			enemyShot.scale = 0.5; // set scale of enemy shot sprite

			int colToShoot; //used to determin wich enemy will shoot
			const int CHANCE_TO_SHOOT = 3; //used to determin frequency of shot

			//create an actor to display explosion
			Actor explode;
			explode.image = LoadImage(".\\data\\laserRed08.png");
			explode.texture = LoadTextureFromImage(explode.image);
			int count = 0;
			const int LENGTH = 15; //amount of time the explode will be displayed		
			enemyShot.image = LoadImage(".\\data\\laserRed07.png");
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
			Sound shootfx = LoadSound(".\\data\\sfx_laser2.ogg");
			Sound boom = LoadSound(".\\data\\mixkit-pixel-chiptune-explosion-1692.wav");
			Music music = LoadMusicStream(".\\data\\Abstract Vision #3 (Looped).wav");
			//play the music
			PlayMusicStream(music);
			SetMusicVolume(music, 0.25f);

    //////////////////////////////
    /// BARRIERS
	//////////////////////////////
			
			
			static const int B_LENGTH = 20; //the length pf each barrier
			static const int B_HEIGHT = 10; //the height of each barrier

			static const int START_X_1 = (GetScreenWidth() * 0.25f) - ((B_LENGTH * Barriers::SIZE) / 2); //set the starting X point for the first barrier
			static const int START_X_2 = (GetScreenWidth() * 0.5f) + ((B_LENGTH * Barriers::SIZE) / 2); //set the starting x point for the second barrier
			static const int START_X_3 = (GetScreenWidth() * 0.75f) + ((B_LENGTH * Barriers::SIZE) / 2); //set the starting x point for the third barrier
			static const int START_Y = GetScreenHeight() * 0.75f; //set the Y point for

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
					// 
					//update music stream
					UpdateMusicStream(music);

					//calculate delta time
					previousTime = currentTime;
					currentTime = GetTime();
					deltaTime = (currentTime - previousTime);


					/////////////////////////////////////////////
					//////Check If Player Has Killed All Enemies
					/////////////////////////////////////////////
					if (kills == (EN_ROWS * EN_COLS))
					{
						kills = 0; //reset kills to 0 for next wave
						enemySpeedUp = 1; //reset the speed boost to enemies
						levelUp = levelUp + 0.2; //incrase the modifier for each time you pass a level
						enemySpeedUp = enemySpeedUp + levelUp; //increase the enemies speed each new level
						playerShot.isDead = true;
						EnemyHasShot = false;
						if (trippleShot)
						{
							extraShots.first.isDead = true;
							extraShots.second.isDead = true;
						}

						paused = true; //pause the game
						if (paused)
						{

							Upgrade(scrap, shootThrough, trippleShot, playerSpeedBoost, shootSpeedBoost); //load updates
							
						}
										

						Reset(enemies, barrier1, barrier2, barrier3); // reset enemy positions		
						cout << enemySpeedUp << endl;
					}

					/////////////////////////////////////////////
					/////Check if player is dead
					/////////////////////////////////////////////
					if (player.isDead)
					{
						//game = false;
						GameOver(scrap, game);
					}
					
					/////////////////////////////////////////////
					/////Fit AABBs
					/////////////////////////////////////////////

					//calculate the new x and y position of the lower right corner of player sprite
					playerMax.x = player.worldPosition.x + (player.texture.width * player.scale);
					playerMax.y = player.worldPosition.y + (player.texture.height * player.scale);

					//refit AABBs
					player.Box.Fit(player.worldPosition, playerMax);					
					//refit enemey AABBs
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

					////calculate the new x and y positios of the lower right corner of the enemy
					//enemy.Max.x = enemy.worldPosition.x + (enemy.texture.width * enemy.scale);
					//enemy.Max.y = enemy.worldPosition.y + (enemy.texture.height * enemy.scale);
					//enemy.Box.Fit(enemy.worldPosition, enemy.Max);

						////////////////////////////////////////
						////Trigger Enemy Shot
						////////////////////////////////////////
					if (!paused)
					{
						if (firstShot)
						{
							if (!EnemyHasShot)
							{
								//seed ramd
								srand((unsigned)time(NULL));
								int willShoot = rand() % CHANCE_TO_SHOOT;

								if (willShoot == (CHANCE_TO_SHOOT - 1))
								{
									colToShoot = rand() % EN_COLS;
									for (int i = 0; i < EN_ROWS; i++)
									{
										if (!enemies[i][colToShoot].isDead)
										{
											enemyShot.worldPosition.y = enemies[i][colToShoot].worldPosition.y + ((enemies[i][colToShoot].texture.height) * enemies[i][colToShoot].scale);
											enemyShot.worldPosition.x = enemies[i][colToShoot].worldPosition.x + ((enemies[i][colToShoot].texture.width / 2) * enemies[i][colToShoot].scale);
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
										//game = false;
										GameOver(scrap, game);
									}
								}
							}
						}

						///check for special win cindition
						if (!firstShot)
						{
							for (int i = 0; i < EN_ROWS; i++)
							{
								for (int j = 0; j < EN_COLS; j++)
								{
									if (enemies[i][j].worldPosition.y + (enemies[i][j].texture.height * enemies[i][j].scale) >= player.worldPosition.y)
									{
										scrap += 99999;
										//game = false;
										GameWin(scrap, game);
									}
								}
							}
						}

						//check if the player has shot
						if (!playerShot.isDead)
						{
							/////////////////////
							//Move the shot
							/////////////////////
							playerShot.worldPosition.y -= ((deltaTime * SHOT_SPEED)) * shootSpeedBoost; //continue to move the shot up the screen every frame
							


							////////////////////////////////////
							/////detect when shot leeves screen
							////////////////////////////////////
							if (playerShot.worldPosition.y <= 0) //if the players shot goes past the top of the screen
							{
								playerShot.isDead = true; //set has shot to false
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
										enemySpeedUp += 0.05;
										enemies[i][j].worldPosition.x = -100;
										enemies[i][j].worldPosition.y = -1000000;
										PlaySound(boom);
										scrap += scrapPerKill;
										kills++;
										if (!shootThrough)
										{
											playerShot.isDead = true;
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
												playerShot.isDead = true;
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
												playerShot.isDead = true;
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
												playerShot.isDead = true;
											}
										}
									}									
								}
							}
						}

						//////////////////////////
						///Tripple Shot Movement
						/////////////////////////
						if (trippleShot)
						{
							if (!extraShots.first.isDead)
							{
								extraShots.first.worldPosition.y -= (deltaTime * SHOT_SPEED); //continue to move the shot up the screen every frame
							}
							if (!extraShots.second.isDead)
							{
								extraShots.second.worldPosition.y -= (deltaTime * SHOT_SPEED); //continue to move the shot up the screen every frame
							}

							////////////////////////////////////////////
							///////Tripple Shot Collisions
							////////////////////////////////////////////
						
							if (trippleShot)
							{
								//check if extra shots are of screen
								if (extraShots.first.worldPosition.y < 0)
								{
									extraShots.first.isDead = true;
								}
								if (extraShots.second.worldPosition.y < 0)
								{
									extraShots.second.isDead = true;
								}

								///////////////////////////////////////
								///////Enemy Collisions
								///////////////////////////////////////
								for (int i = 0; i < EN_ROWS; i++)
								{
									for (int j = 0; j < EN_COLS; j++)
									{
										if (enemies[i][j].Box.Overlaps(extraShots.first.worldPosition))
										{
											explode.worldPosition.x = enemies[i][j].worldPosition.x + (enemies[i][j].texture.width * enemies[i][j].scale) / 2;
											explode.worldPosition.y = enemies[i][j].worldPosition.y + (enemies[i][j].texture.height * enemies[i][j].scale) / 2;
											explode.isDead = false;
											enemies[i][j].isDead = true;
											enemySpeedUp += 0.05;
											enemies[i][j].worldPosition.x = -100;
											enemies[i][j].worldPosition.y = -1000000;
											PlaySound(boom);
											scrap += scrapPerKill;
											kills++;

											extraShots.first.worldPosition.y = -100;
											extraShots.second.isDead;
										}

										if (enemies[i][j].Box.Overlaps(extraShots.second.worldPosition))
										{
											explode.worldPosition.x = enemies[i][j].worldPosition.x + (enemies[i][j].texture.width * enemies[i][j].scale) / 2;
											explode.worldPosition.y = enemies[i][j].worldPosition.y + (enemies[i][j].texture.height * enemies[i][j].scale) / 2;
											explode.isDead = false;
											enemies[i][j].isDead = true;
											enemySpeedUp += 0.05;
											enemies[i][j].worldPosition.x = -100;
											enemies[i][j].worldPosition.y = -1000000;
											PlaySound(boom);
											scrap += scrapPerKill;
											kills++;

											extraShots.second.worldPosition.y = -100;
											extraShots.second.isDead;
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
											if (trippleShot)
											{
												if (barrier1[i][j].Box.Overlaps(extraShots.first.worldPosition))
												{
													PlaySound(boom);
													barrier1[i][j].isDead = true;
													barrier1[i][j].worldPosition.x = -100;
													extraShots.first.worldPosition.y = -100;
													extraShots.first.isDead;
												}
												if (barrier1[i][j].Box.Overlaps(extraShots.second.worldPosition))
												{
													PlaySound(boom);
													barrier1[i][j].isDead = true;
													barrier1[i][j].worldPosition.x = -100;
													extraShots.second.worldPosition.y = -100;
													extraShots.second.isDead;
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
											
											if (trippleShot)
											{
												if (barrier2[i][j].Box.Overlaps(extraShots.first.worldPosition))
												{
													PlaySound(boom);
													barrier2[i][j].isDead = true;
													barrier2[i][j].worldPosition.x = -100;
													extraShots.first.worldPosition.y = -100;
													extraShots.first.isDead;
												}
												if (barrier2[i][j].Box.Overlaps(extraShots.second.worldPosition))
												{
													PlaySound(boom);
													barrier2[i][j].isDead = true;
													barrier2[i][j].worldPosition.x = -100;
													extraShots.second.worldPosition.y = -100;
													extraShots.second.isDead;
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

											if (trippleShot)
											{
												if (barrier3[i][j].Box.Overlaps(extraShots.first.worldPosition))
												{
													PlaySound(boom);
													barrier3[i][j].isDead = true;
													barrier3[i][j].worldPosition.x = -100;
													extraShots.first.worldPosition.y = -100;
													extraShots.first.isDead;
												}
												if (barrier3[i][j].Box.Overlaps(extraShots.second.worldPosition))
												{
													PlaySound(boom);
													barrier3[i][j].isDead = true;
													barrier3[i][j].worldPosition.x = -100;
													extraShots.second.worldPosition.y = -100;
													extraShots.second.isDead;
												}
											}
										}
									}
								}


							}
						}

						///////////////////////////////////////
						/////Enemy Shot Movement And Collisions
						///////////////////////////////////////
						if (EnemyHasShot)
						{
							enemyShot.worldPosition.y += (deltaTime * ENEMY_SHOT_SPEED);
							if (enemyShot.worldPosition.y >= GetScreenHeight())
							{
								EnemyHasShot = false;
							}
							
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
										enemies[i][j].worldPosition.x += ((deltaTime * ENEMY_SPEED) * enemySpeedUp);

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
										enemies[i][j].worldPosition.x -= ((deltaTime * ENEMY_SPEED) * enemySpeedUp);
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

								}
							}
						}
					}

					/////////////////////////////////////////////////////
					///Get User Inpiut
					/////////////////////////////////////////////////////
					if (!paused)
					{
						if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
						{
							if (player.worldPosition.x + player.texture.width >= GetScreenWidth())
							{

							}
							else
							{
								player.worldPosition.x += ((deltaTime * PLAYER_SPEED)) * playerSpeedBoost;
							}
						}
						if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
						{
							if (player.worldPosition.x <= 0)
							{

							}
							else
							{
								player.worldPosition.x -= ((deltaTime * PLAYER_SPEED)) * playerSpeedBoost;
							}

						}

						if (IsKeyDown(KEY_SPACE))
						{
							if (playerShot.isDead)
							{
								playerShot.isDead = false;
								firstShot = true;
								playerShot.worldPosition.x = player.worldPosition.x + (player.texture.width / 2) - (playerShot.texture.width / 2);
								playerShot.worldPosition.y = player.worldPosition.y - (playerShot.texture.height / 2);
								PlaySound(shootfx);

								
								if (trippleShot)
								{
									if (extraShots.first.isDead)
									{
										extraShots.first.isDead = false;
										extraShots.first.worldPosition.x = player.worldPosition.x + (playerShot.texture.width / 2);
										extraShots.first.worldPosition.y = player.worldPosition.y + (playerShot.texture.height * playerShot.scale);
									}

									if (extraShots.second.isDead)
									{
										extraShots.second.isDead = false;
										extraShots.second.worldPosition.x = (player.worldPosition.x + player.texture.width) - (playerShot.texture.width / 2);
										extraShots.second.worldPosition.y = player.worldPosition.y + (playerShot.texture.height * playerShot.scale);
									}
								}
							}


						}

						if (IsKeyPressed(KEY_ESCAPE))///if escape key is pressed
						{
							
							game = false; //end the loop
						}
					}
					if (IsKeyPressed(KEY_P))///if escape key is pressed
					{
						paused = false;
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
					//////////////////////////////////////////
					//////Draw Player
					/////////////////////////////////////////
					if (!player.isDead)
					{
						//player.Box.DebugBox(RED);
						player.Draw();
					}
					/////////////////////////////////////////
					//////Draw Players Shot
					/////////////////////////////////////////
					if (!playerShot.isDead) //check if the player has shot
					{
						
						playerShot.Draw();					

					}
					if (trippleShot)
					{
						if (!extraShots.first.isDead)
						{
							extraShots.first.Draw();
						}
						if (!extraShots.second.isDead)
						{
							extraShots.second.Draw();
						}
					}


					////////////////////////////
					//////Draw Enemy
					////////////////////////////
					for (int i = 0; i < EN_ROWS; i++)
					{
						for (int j = 0; j < EN_COLS; j++)
						{
							if (!enemies[i][j].isDead)
							{
								//enemies[i][j].Box.DebugBox(GREEN);
								enemies[i][j].Draw();
							}
						}
					}

					/////////////////////////////////
					///////Draw Enemy Shot
					////////////////////////////////
					if (EnemyHasShot)
					{
						enemyShot.Draw();
					}

					///////////////////////////////////////
					/////Draw Explode icon
					///////////////////////////////////////
					if (!explode.isDead)
					{
						count++; //calculate the amount of time the explosion has been on screen
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

						if (paused)
						{
							DrawText("PAUSED", (GetScreenWidth() / 2) - (MeasureText("PAUSED", 50) / 2), GetScreenHeight() / 2, 50, RED);
							DrawText("Press P To Continue", (GetScreenWidth() / 2) - (MeasureText("Press P To Continue", 30) / 2), GetScreenHeight() / 2 + 70, 30, RED);
						}
						EndDrawing();				
										
				}	
				////////////////////////
				////Exit
				////////////////////////
				if (!game)
				{
					CloseWindow(); //close the game window
				}
}

void Upgrade(int &scrap, bool &shootThrough, bool &trippleShot, float& playerSpeedBoost, float &shootSpeedBoost)
{
	bool Upgraded = false;

	while (!Upgraded)
	{		
		BeginDrawing();	

		ClearBackground(BLACK);

		DrawText(TextFormat("Scrap: %05i", scrap), (GetScreenWidth() / 2) - 50, 10, 30, WHITE);
		DrawText("Press Esc To Skip", 10, 30, 30, WHITE);
		DrawText("Press 1 to Get Shoot Through Upgrade (500 Scrap)", 10, 100, 30, WHITE);
		DrawText("Press 2 to Get Tripple Shot Upgrade (500 Scrap)", 10, 150, 30, WHITE);
		DrawText("Press 3 to Get Movement Speed Upgrade (500 Scrap)", 10, 200, 30, WHITE);
		DrawText("Press 4 to Get Bullet Speed Upgrade (500 Scrap)", 10, 250, 30, WHITE);
		if (IsKeyPressed(KEY_ESCAPE))///if escape key is pressed
		{
			Upgraded = true;
			
		}
		if (scrap > 500)
		{
			if (IsKeyPressed(KEY_ONE))///if 1 is pressed
			{
				shootThrough = true;
				scrap -= 500;
				Upgraded = true;

			}
			if (IsKeyPressed(KEY_TWO))///if 2 is pressed
			{
				trippleShot = true;
				scrap -= 500;
				Upgraded = true;

			}
			if (IsKeyPressed(KEY_THREE))///if 2 is pressed
			{
				playerSpeedBoost += 0.1;
				scrap -= 500;
				Upgraded = true;

			}
			if (IsKeyPressed(KEY_FOUR))///if 2 is pressed
			{
				shootSpeedBoost += 0.1;
				scrap -= 500;
				Upgraded = true;

			}
		}
		else
		{
			DrawText("Sorry, you do not have enough scrap", 10, GetScreenHeight() - 100, 30, WHITE);
		}

		EndDrawing();		
	}	
	
}

void Reset(vector<vector<Actor>> &enemies, vector<vector<Barriers>> &barrier1, vector<vector<Barriers>> &barrier2, vector<vector<Barriers>> &barrier3)
{
			//////////////////////
			///Set enemies
			//////////////////////

	int lastX = 0;
	int lastY = 0;
	for (int i = 0; i < EN_ROWS; i++)
	{
		for (int j = 0; j < EN_COLS; j++)
		{   
			//set x and y positions for the enemy
			enemies[i][j].worldPosition.x = lastX + (enemies[0][0].texture.width);
			enemies[i][j].worldPosition.y = lastY + (enemies[0][0].texture.height);

			lastX = enemies[i][j].worldPosition.x;

			//add enemy to array
			enemies[i][j].isDead = false;
		}
		lastX = 0;
		lastY = lastY + enemies[0][0].texture.height;
	}

	static const int START_X_1 = (GetScreenWidth() * 0.25f) - ((B_LENGTH * Barriers::SIZE) / 2); //set the starting X point for the first barrier
	static const int START_X_2 = (GetScreenWidth() * 0.5f) + ((B_LENGTH * Barriers::SIZE) / 2); //set the starting x point for the second barrier
	static const int START_X_3 = (GetScreenWidth() * 0.75f) + ((B_LENGTH * Barriers::SIZE) / 2); //set the starting x point for the third barrier
	static const int START_Y = GetScreenHeight() * 0.75f; //set the Y point for all barriers


	////////////////////////////////////////
	////Set up Barrier 1
	////////////////////////////////////////

	int nextX = START_X_1; //used to calculate where to put the next peice on the x
	int nextY = START_Y; //used to calculate where to put the next peice on the y
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
}

void GameOver(int scrap, bool &game)
{
	while (game)
	{
		BeginDrawing();

		ClearBackground(BLACK);


		DrawText(TextFormat("Scrap: %05i", scrap), (GetScreenWidth() / 2) - 50, 10, 20, WHITE);
		DrawText("GAME OVER", (GetScreenWidth() / 2) - (MeasureText("GAME OVER", 50) / 2), GetScreenHeight() / 2, 50, WHITE);

		EndDrawing();

		if (IsKeyPressed(KEY_ESCAPE))///if escape key is pressed
		{
			game = false; //end the loop
		}
		
	}
}

void GameWin(int scrap, bool &game)
{
	while (game)
	{
		BeginDrawing();

		ClearBackground(BLACK);
		string message = "The alien ships reach you and you discuss tearms of a treaty. \n They offer to work with you to better the human race. \n They also bring you cake.";

		DrawText(TextFormat("Scrap: %05i", scrap), (GetScreenWidth() / 2) - 50, 10, 20, WHITE);
		DrawText(message.c_str(), (GetScreenWidth() / 2) - (MeasureText(message.c_str(), 50) / 2), GetScreenHeight() / 2, 50, WHITE);

		EndDrawing();

		if (IsKeyPressed(KEY_ESCAPE))///if escape key is pressed
		{			
			game = false; //end the loop
		}

		
	}
}