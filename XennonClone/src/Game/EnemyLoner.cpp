#include "EnemyLoner.h"
#include "WEngine/GameContext.h"
#include "EnemyProjectile.h"
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

	shotCooldownTime = 2.5f;
	projectileSpeed = 10.f;


}

void EnemyLoner::Start()
{
	Enemy::Start();

	float positionOffsetSign = (int)(GetLocation().x > 0) - (int)(0 > GetLocation().x);
	moveSpeed = 2.5f * -positionOffsetSign;



}

void EnemyLoner::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	Move(WVec2(moveSpeed, 0));

	HandleShoot(deltaTime);

	HandleEnemyLifeTime();

	//Move(WVec2(sin(GetTimeAlive() * 4) * 3, -moveSpeed));

}


void EnemyLoner::Fire()
{
	EnemyProjectile* projectile = GetGameContext()->GAME_InstantiateEntity<EnemyProjectile>(GetLocation() + WVec2(0, -1), WVec2(2));
	GetGameContext()->PHYS_SetLinearVelocityOnPhysObj(projectile->bodyId, WVec2(0, -projectileSpeed));
}





