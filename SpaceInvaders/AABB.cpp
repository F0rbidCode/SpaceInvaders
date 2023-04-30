#include "AABB.h"

//create functions to fit the AABB
void AABB::Fit(Vector2 min, Vector2 max)
{
	this->min = min;
	this->max = max;
}
//create function to check for a point overlapping with aabb
bool AABB::Overlaps(Vector2 p)
{
	return !(p.x < min.x || p.y < min.y || p.x > max.x || p.y > max.y);
}

//create a function to draw the AABB onscreen for debugging
void AABB::DebugBox(Color c)
{
	DrawRectangle((int)min.x, (int)min.y, (int)max.x - (int)min.x, (int)max.y - (int)min.y, c);
}
