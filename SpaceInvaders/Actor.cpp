#include "Actor.h"

void Actor::Draw()
{
	DrawTextureEx(texture, worldPosition, 0, scale, WHITE);
}