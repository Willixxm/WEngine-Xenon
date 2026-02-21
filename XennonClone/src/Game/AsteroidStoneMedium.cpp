#include "AsteroidStoneMedium.h"

AsteroidStoneMedium::AsteroidStoneMedium()
{
	filePath = "graphics/SAster64.bmp";
	hitboxSizeMult = 0.8f;
	hTiles = 8;
	vTiles = 3;
	tileOffset = 0;
	tileSpan = hTiles * vTiles;
	renderLayer = 0;
	animationFPS = 9.f;

	lifePoints = 120.f;

	moveSpeed = 0.f;
}

void AsteroidStoneMedium::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	Move(normalizedMoveVector * moveSpeed);
}