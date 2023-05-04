#include "Barriers.h"

void Barriers :: MakeBarriers()
{
	worldPosition.x = GetScreenWidth() /2;
	worldPosition.y = GetScreenHeight() * .75;
	isDead = false;
	
}

 void Barriers :: DrawBarriers()
{
	DrawRectangle((int)worldPosition.x, (int)worldPosition.y, SIZE, SIZE, GREEN);
}
