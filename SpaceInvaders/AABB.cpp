#include "AABB.h"

//create functions to fit the AABB

template <class T>
void AABB<T>::Fit(Vector2 min, Vector2 max)
{
	this->min = min;
	this->max = max;
}
//create function to check for a point overlapping with aabb

template <class T>
bool AABB<T>::Overlaps(Vector2 p)
{
	return !(p.x < min.x || p.y < min.y || p.x > max.x || p.y > max.y);
}

//create a function to draw the AABB onscreen for debugging

template <class T>
void AABB<T>::DebugBox(Color c)
{
	DrawRectangle((int)min.x, (int)min.y, (int)max.x - (int)min.x, (int)max.y - (int)min.y, c);
}
