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

	GetGameContext()->PHYS_AddPhysComponentToEntity(this, WE::WBodyType::dynamicBody, GetInitialSize() * hitboxSizeMult, WCollisionLayer::Layer2, WCollisionLayer::Layer1, isSensor, 1.f);
	//GetGameContext()->PHYS_SetLinearVelocityOnPhysObj(this->bodyId, WE::WVec2(0, -2));
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
		Die();
	}

}

void Enemy::Die()
{
	GetGameContext()->GAME_InstantiateEntity<Explosion>(GetLocation(), GetInitialSize());

	//TODO give points

	Destroy();
}

void Enemy::On_SensorBeginOverlap(Entity* other)
{
	Entity::On_SensorBeginOverlap(other);


	SpaceShip* hitEnemy = dynamic_cast<SpaceShip*>(other);
	if (hitEnemy)
	{
		hitEnemy->DealDamage(this, bodyDamage);

		if (isSensor)
			Die();

	}

}