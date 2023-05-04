#pragma once
#include "AABB.h"
#include "Source.cpp"
class Barriers
{
public:
	Vector2 worldPosition; //used to store world position
	AABB Box; //create an AABB for enemy collisions
	bool isDead = false; //used to determin if actor is still alive
private:
    const int SIZE;
};

