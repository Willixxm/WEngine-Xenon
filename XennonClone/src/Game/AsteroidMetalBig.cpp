#include "AsteroidMetalBig.h"

AsteroidMetalBig::AsteroidMetalBig()
{
	filePath = "graphics/MAster96.bmp";
	hitboxSizeMult = 0.8f;
	hTiles = 5;
	vTiles = 5;
	tileOffset = 0;
	tileSpan = hTiles * vTiles;
	renderLayer = 0;
	animationFPS = 9.f;

	lifePoints = 9999.f;
	isInvincible = true;

	moveSpeed = 0.f;
}

void AsteroidMetalBig::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	Move(normalizedMoveVector * moveSpeed);
}