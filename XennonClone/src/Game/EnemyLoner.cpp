#include "EnemyLoner.h"
#include "WEngine/GameContext.h"
#include "EnemyProjectile.h"
#include "XennonLevel.h"
#include <cmath>





EnemyLoner::EnemyLoner()
{
	
	
	//SetLocation(WVec2(25 * positionOffsetSign, GetLocation().y));

	filePath = "graphics/LonerA.bmp";
	hitboxSizeMult = 0.6f;
	hTiles = 4;
	vTiles = 4;
	tileOffset = 0;
	tileSpan = hTiles * vTiles;
	renderLayer = 0;
	animationFPS = 5.f;

	lifePoints = 75.f;


	shotCooldownTime = 2.0f;
	projectileSpeed = 10.f;

	moveSpeed = 6.f;
	normalizedMoveVector = WVec2(-1, 0);


}

void EnemyLoner::Start()
{
	Enemy::Start();

	//float positionOffsetSign = (int)(GetLocation().x > 0) - (int)(0 > GetLocation().x);
	//moveSpeed = 2.5f * -positionOffsetSign;

}

void EnemyLoner::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	Move(normalizedMoveVector * moveSpeed);

	HandleShoot(deltaTime);

	HandleEnemyLifeTime();

	
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


