#include "Enemy.h"
#include "WEngine/GameContext.h"
#include "WEngine/Pawn.h"
#include "PlayerProjectile.h"
#include "SpaceShip.h"
#include "Explosion.h"
#include "XennonLevel.h"
#include "TextRenderer.h"

#include "iostream"
#include <string>


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

	HandleShoot(deltaTime);
	HandleEnemyLifeTime();
}


void Enemy::DealDamage(Entity* dealer, float damage)
{
	if (isInvincible)
		return;

	Pawn::DealDamage(dealer, damage);

	lifePoints -= damage;

	if (lifePoints <= 0 && !hasDied)
	{
		hasDied = true;
		lifePoints = 0;
		DieByPlayer();
	}
}

void Enemy::DieByPlayer()
{
	GetGameContext()->GAME_InstantiateEntity<Explosion>(GetLocation(), (rand() % 618) * 0.01f, GetInitialSize() * explosionSizeMult);

	if (GetLevelInstance())
	{
		GetLevelInstance()->AddScore(score);
	}

	auto scoreText = GetGameContext()->GAME_InstantiateEntity<TextRenderer>(GetLocation() + WVec2(0, 1), 0, WVec2(0.6));
	scoreText->SetFont(false);
	scoreText->SetText(std::to_string(score));
	scoreText->DestroyWithFloat(WVec2(0, 2), 0.5);

	Destroy();
}

void Enemy::On_SensorBeginOverlap(Entity* other)
{
	Entity::On_SensorBeginOverlap(other);

	if (!other)
		return;

	extraLifeTime = GetTimeAlive();
	
	SpaceShip* hitPlayer = dynamic_cast<SpaceShip*>(other);
	if (hitPlayer)
	{
		hitPlayer->DealDamage(this, bodyDamage);
		DealDamage(hitPlayer, 9999.f); //receive damage
	}
}

void Enemy::On_CollisionBegin(Entity* other, WVec2 point)
{
	Entity::On_CollisionBegin(other, point);

	if (!other)
		return;

	extraLifeTime = GetTimeAlive();
	
	SpaceShip* hitPlayer = dynamic_cast<SpaceShip*>(other);
	if (hitPlayer)
	{
		hitPlayer->DealDamage(this, bodyDamage);
		DealDamage(hitPlayer, 9999.f); //receive damage
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
	if (GetTimeAlive() > maxLifeTime + extraLifeTime)
	{
		Destroy();
	}
}