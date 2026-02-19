#include "Enemy.h"
#include "WEngine/GameContext.h"
#include "WEngine/Pawn.h"
#include "PlayerProjectile.h"
#include "SpaceShip.h"
#include "Explosion.h"

#include "iostream"


using namespace WE;

void Enemy::Start()
{
	Pawn::Start();

	GetGameContext()->PHYS_AddPhysComponentToEntity(this, WE::WBodyType::kinematicBody, GetInitialSize() * hitboxSizeMult, collisionLayer, collidesWith, isSensor, 1.f);
	GetGameContext()->RENDER_AddRenderComponent(this, filePath, hTiles, vTiles, tileOffset, tileSpan, renderLayer);
	GetGameContext()->RENDER_SetAnimationParameters(this, true, animationFPS);

}


void Enemy::Update(float deltaTime)
{
	Pawn::Update(deltaTime);

}


void Enemy::DealDamage(Entity* dealer, float damage)
{
	Pawn::DealDamage(dealer, damage);

	lifePoints -= damage;

	if (lifePoints <= 0)
	{
		DieByPlayer();
	}
}

void Enemy::DieByPlayer()
{
	GetGameContext()->GAME_InstantiateEntity<Explosion>(GetLocation(), 0.f, GetInitialSize());

	//TODO give points

	Destroy();
}

void Enemy::On_SensorBeginOverlap(Entity* other)
{
	Entity::On_SensorBeginOverlap(other);

	if (!other)
		return;

	SpaceShip* hitPlayer = dynamic_cast<SpaceShip*>(other);
	if (hitPlayer)
	{
		hitPlayer->DealDamage(this, bodyDamage);

		if (isSensor)
			DieByPlayer();
	}

}

void Enemy::HandleShoot(float deltaTime)
{
	timeUntilNextShot -= deltaTime;
	if (timeUntilNextShot <= 0 && canShoot)
	{
		timeUntilNextShot = shotCooldownTime;

		Fire();
	}
}

void Enemy::Fire()
{

}

void Enemy::HandleEnemyLifeTime()
{
	if (GetTimeAlive() > maxLifeTime)
	{
		Destroy();
	}
}