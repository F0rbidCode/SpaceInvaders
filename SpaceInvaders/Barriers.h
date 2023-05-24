#pragma once
#include "AABB.h"
#include "Actor.h"

class Barriers
{
public:	
	Vector2 worldPosition; //used to store world position
	Vector2 Max; //used to store the lower right corner of the barier box
	AABB<int> Box; //create an AABB for enemy collisions
	bool isDead = false; //used to determin if actor is still alive
    static const int SIZE = 10;

	Barriers() {}
	~Barriers() {}


public:
	void MakeBarriers(int x, int y);
	void DrawBarriers();
};

