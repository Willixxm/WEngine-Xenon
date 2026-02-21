#include "AsteroidStoneBig.h"

AsteroidStoneBig::AsteroidStoneBig()
{
	filePath = "graphics/SAster96.bmp";
	hitboxSizeMult = 0.8f;
	hTiles = 5;
	vTiles = 5;
	tileOffset = 0;
	tileSpan = hTiles * vTiles;
	renderLayer = 0;
	animationFPS = 9.f;

	lifePoints = 200.f;

	moveSpeed = 0.f;
}

void AsteroidStoneBig::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	Move(normalizedMoveVector * moveSpeed);
}
