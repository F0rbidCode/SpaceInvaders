#pragma once
#include <raylib.h>
#include "AABB.h"


class Actor
{
public:

		Vector2 worldPosition; //used to store world position
		Vector2 Max;
		AABB<int> Box; //create an AABB for enemy collisions
		Texture2D texture; // used to store texture
		Image image; // used to store image
		bool isDead = false; //used to determin if actor is still alive
		float scale; //used to set the scale of the actor
		//int count = 0; //used to determing the length of time the explosion stays on the screen

		Actor() {}

		//Overload Constructor
		Actor(int screenHeight, int screenWidth,  Image im, float sc)
	{
		//load imgae and texture
		image = im;
		texture = LoadTextureFromImage(image);

		isDead = false;//set is dead to false
		scale = sc; //set scale

		//set x and y positions of the actor
		worldPosition.x = (GetScreenWidth() / 2) - (texture.width / 2) * scale;
		worldPosition.y = (GetScreenHeight() - (texture.height * 2)) * scale; 

		//calculate the new x and y position of the lower right corner of actor sprite
		Max.x = worldPosition.x + (texture.width * scale);
		Max.y = worldPosition.y + (texture.height * scale);		

		//fit the AABB to the actor
		Box.Fit(worldPosition, Max);
	}

		~Actor() {}

		void Draw();
		
};

