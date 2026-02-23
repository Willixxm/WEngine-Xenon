#include "EnemyRusher.h"

EnemyRusher::EnemyRusher()
{
	filePath = "graphics/rusher.bmp";
	hitboxSizeMult = 0.4f;
	hTiles = 4;
	vTiles = 6;
	tileOffset = 0;
	tileSpan = hTiles * vTiles;
	renderLayer = 0;
	animationFPS = 15.f;

	explosionSizeMult = 1.0f;

	lifePoints = 50.f;
	moveSpeed = 5.f;
	moveVector = WVec2(0, -1);
	
	maxLifeTime = 15.f;

	score = 10000;
}

void EnemyRusher::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	Move(WVec2(-3.3f, moveSpeed));	
}
