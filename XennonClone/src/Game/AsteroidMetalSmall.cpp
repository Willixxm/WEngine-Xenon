#include "AsteroidMetalSmall.h"

AsteroidMetalSmall::AsteroidMetalSmall()
{
	filePath = "graphics/MAster32.bmp";
	hitboxSizeMult = 0.8f;
	hTiles = 8;
	vTiles = 2;
	tileOffset = 0;
	tileSpan = hTiles * vTiles;
	renderLayer = 0;
	animationFPS = 9.f;

	lifePoints = 9999.f;
	isInvincible = true;

	moveSpeed = 0.f;
}

void AsteroidMetalSmall::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	Move(normalizedMoveVector * moveSpeed);
}