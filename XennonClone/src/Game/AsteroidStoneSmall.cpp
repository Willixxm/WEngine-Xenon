#include "AsteroidStoneSmall.h"

AsteroidStoneSmall::AsteroidStoneSmall()
{
	filePath = "graphics/SAster32.bmp";
	hitboxSizeMult = 0.8f;
	hTiles = 8;
	vTiles = 2;
	tileOffset = 0;
	tileSpan = hTiles * vTiles;
	renderLayer = 0;
	animationFPS = 9.f;

	lifePoints = 60.f;

	moveSpeed = 0.f;
}

void AsteroidStoneSmall::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	Move(normalizedMoveVector * moveSpeed);
}