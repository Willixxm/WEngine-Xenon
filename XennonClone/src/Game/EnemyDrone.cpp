#include "EnemyDrone.h"
#include <iostream>

EnemyDrone::EnemyDrone()
{
	filePath = "graphics/drone.bmp";
	hitboxSizeMult = 0.7f;
	hTiles = 8;
	vTiles = 2;
	tileOffset = 0;
	tileSpan = hTiles * vTiles;
	renderLayer = 0;
	animationFPS = 15.f;

	lifePoints = 50.f;
	moveSpeed = 6.f;
	normalizedMoveVector = WVec2(0, -1);

	maxLifeTime = 12.f;
}

void EnemyDrone::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	float oscillator = sin(GetTimeAlive() * 3.14f);
	float sign = std::copysign(1.0f, oscillator);
	float sideWaysForce = sign * pow(abs(oscillator), 1/1.2f);
	
	Move(normalizedMoveVector * moveSpeed + WVec2(sideWaysForce, 0) * moveSpeed*1.5f);
}