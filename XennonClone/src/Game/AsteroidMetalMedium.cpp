#include "AsteroidMetalMedium.h"

AsteroidMetalMedium::AsteroidMetalMedium()
{
	filePath = "graphics/MAster64.bmp";
	hitboxSizeMult = 0.8f;
	hTiles = 8;
	vTiles = 3;
	tileOffset = 0;
	tileSpan = hTiles * vTiles;
	renderLayer = 0;
	animationFPS = 9.f;

	lifePoints = 9999.f;
	isInvincible = true;

	moveSpeed = 0.f;
}

void AsteroidMetalMedium::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	Move(normalizedMoveVector * moveSpeed);
}