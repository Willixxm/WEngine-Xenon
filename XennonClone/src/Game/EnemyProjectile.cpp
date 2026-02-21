#include "EnemyProjectile.h"
#include "WEngine/GameContext.h"
#include <cmath>

#include "Explosion2.h"

EnemyProjectile::EnemyProjectile()
{
	float positionOffsetSign = ((float)std::signbit(GetLocation().x) - 0.5f) * 2;

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
	maxLifeTime = 7.f;
}

void EnemyProjectile::Update(float deltaTime)
{
	Enemy::Update(deltaTime);
	HandleEnemyLifeTime();
}

void EnemyProjectile::DieByPlayer()
{
	GetGameContext()->GAME_InstantiateEntity<Explosion2>(GetLocation(), 0.f, GetInitialSize());

	Destroy();
}





