#pragma once
#include <iostream>
#include "raylib.h"
#include "raymath.h"

class AABB
{
	Vector2 min;
	Vector2 max;


public:
	void Fit(Vector2 min, Vector2 max);

	bool Overlaps(Vector2 p);

	void DebugBox(Color c);
};


