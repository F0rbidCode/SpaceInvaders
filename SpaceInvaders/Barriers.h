#pragma once
#include "AABB.h"
#include "Actor.h"
class Barriers
{
public:
	Vector2 worldPosition; //used to store world position
	AABB Box; //create an AABB for enemy collisions
	bool isDead = false; //used to determin if actor is still alive
    const int SIZE = 20;

public:
	void MakeBarriers();
	void DrawBarriers();
};

