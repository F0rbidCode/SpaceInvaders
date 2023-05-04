#include "Barriers.h"

void Barriers()
{
	Actor barrier;

	barrier.scale = 1;
	barrier.worldPosition.x = GetScreenWidth();
	barrier.worldPosition.y = GetScreenHeight() * .75;
	barrier.isDead = false;
	const int SIZE = 20;

}

void DrawBarriers(Actor barrier)
{
	DrawRectangle((int)barrier.worldPosition.x, (int)barrier.worldPosition.y, SIZE, 20, GREEN);
}
