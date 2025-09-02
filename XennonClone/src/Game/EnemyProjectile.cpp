#include "EnemyProjectile.h"
#include "WEngine/GameContext.h"
#include <cmath>





EnemyProjectile::EnemyProjectile()
{
	float positionOffsetSign = ((float)std::signbit(GetLocation().x) - 0.5f) * 2;

	//SetLocation(WVec2(25 * positionOffsetSign, GetLocation().y));

	filePath = "graphics/EnWeap6.bmp";
	hitboxSizeMult = 0.6f;
	hTiles = 8;
	vTiles = 1;
	tileOffset = 0;
	tileSpan = hTiles * vTiles;
	renderLayer = 0;
	animationFPS = 10.f;

	lifePoints = 50.f;

	collidesWith = WCollisionLayer::Layer1;
	maxLifeTime = 10.f;
}



void EnemyProjectile::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	//Move(WVec2(moveSpeed, 0));

	HandleEnemyLifeTime();

	//Move(WVec2(sin(GetTimeAlive() * 4) * 3, -moveSpeed));

}






