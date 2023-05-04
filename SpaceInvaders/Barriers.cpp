#include "Barriers.h"

void Barriers :: MakeBarriers(int x, int y)
{
	worldPosition.x = x;
	worldPosition.y = y;
	isDead = false;
	Max.x = x + SIZE;
	Max.y = y + SIZE;

	Box.Fit(worldPosition, Max);
}

 void Barriers :: DrawBarriers()
{
	DrawRectangle((int)worldPosition.x, (int)worldPosition.y, SIZE, SIZE, GREEN);
}
