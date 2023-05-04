#pragma once
#include <raylib.h>
#include "AABB.h"

class Actor
{
public:
		Vector2 worldPosition; //used to store world position
		Vector2 Max;
		AABB Box; //create an AABB for enemy collisions
		Texture2D texture; // used to store texture
		Image image; // used to store image
		bool isDead = false; //used to determin if actor is still alive
		float scale; //used to set the scale of the actor
		//int count = 0; //used to determing the length of time the explosion stays on the screen

		void Draw();
		
};

