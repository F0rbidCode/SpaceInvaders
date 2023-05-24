#pragma once
#include <iostream>
#include "raylib.h"
#include "raymath.h"

template <class T>
class AABB
{	
	Vector2 min;
	Vector2 max;


public:
	
	AABB() {}
	
	~AABB() {}

	
	void Fit(Vector2 min, Vector2 max);

	
	bool Overlaps(Vector2 p);

	void DebugBox(Color c);
	
};


