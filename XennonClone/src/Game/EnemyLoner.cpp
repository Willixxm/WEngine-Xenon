#include "EnemyLoner.h"
#include "WEngine/GameContext.h"
#include "EnemyProjectile.h"
#include "XennonLevel.h"
#include <cmath>





EnemyLoner::EnemyLoner()
{
	

	filePath = "graphics/LonerA.bmp";
	hitboxSizeMult = 0.6f;
	hTiles = 4;
	vTiles = 4;
	tileOffset = 0;
	tileSpan = hTiles * vTiles;
	renderLayer = 0;
	animationFPS = 5.f;

	lifePoints = 75.f;
	maxLifeTime = 25.0f;

	canShoot = true;
	shotCooldownTime = 2.0f;
	projectileSpeed = 10.f;

	timeUntilNextShot = (rand() % 100) * (1/100.f) * shotCooldownTime;

	moveSpeed = 6.f;
	moveVector = WVec2(-1, 0);

	score = 15000;

}

void EnemyLoner::Start()
{
	Enemy::Start();


}

void EnemyLoner::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	Move(moveVector * moveSpeed);	
}


void EnemyLoner::Fire()
{
	if (!GetLevelInstance())
		return;

	auto playerDirection = GetLevelInstance()->GetPlayer()->GetLocation() - GetLocation();
	if (playerDirection.length() < 45)
	{
		EnemyProjectile* projectile = GetGameContext()->GAME_InstantiateEntity<EnemyProjectile>(GetLocation() + WVec2(0, -1), GetRotation(), WVec2(2));
		GetGameContext()->PHYS_SetLinearVelocityOnPhysObj(projectile->bodyId, playerDirection.normalized() * projectileSpeed);
	}
}


