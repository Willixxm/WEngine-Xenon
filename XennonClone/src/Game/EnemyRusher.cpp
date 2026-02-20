#include "EnemyRusher.h"

EnemyRusher::EnemyRusher()
{
	SetLocation(WVec2(GetLocation().x, 20));
	
	filePath = "graphics/rusher.bmp";
	hitboxSizeMult = 0.4f;
	hTiles = 4;
	vTiles = 6;
	tileOffset = 0;
	tileSpan = hTiles * vTiles;
	renderLayer = 0;
	animationFPS = 15.f;

	lifePoints = 50.f;
	moveSpeed = 5.f;
	normalizedMoveVector = WVec2(0, -1);

	maxLifeTime = 15.f;
}

void EnemyRusher::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	Move(normalizedMoveVector * moveSpeed);

	HandleEnemyLifeTime();
	
}
