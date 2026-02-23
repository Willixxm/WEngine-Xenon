#include "PlayerProjectile.h"
#include "WEngine/Entity.h"
#include "WEngine/WMath.h"
#include "WEngine/GameContext.h"
#include "Enemy.h"
#include "Explosion2.h"

#include <iostream>

void PlayerProjectile::Start()
{
	WE::Entity::Start();

	GetGameContext()->PHYS_AddPhysComponentToEntity(this, WE::WBodyType::kinematicBody, GetInitialSize(), WCollisionLayer::Layer2, WCollisionLayer::Layer3, false, 0.05f);
	GetGameContext()->PHYS_SetLinearVelocityOnPhysObj(this->bodyId, GetUpVector() * projectileSpeed);
	GetGameContext()->RENDER_AddRenderComponent(this, "graphics/missile.bmp", 2, 3, 0, 2, -1);
	GetGameContext()->RENDER_SetAnimationParameters(this, true, 5.f);


}


void PlayerProjectile::Update(float deltaTime)
{
	WE::Entity::Update(deltaTime);

	if (GetTimeAlive() > lifeTime)
	{
		Destroy();
	}
		

}

void PlayerProjectile::On_CollisionBegin(Entity* other, WVec2 hitLocation)
{
	WE::Entity::On_SensorBeginOverlap(other);

	Enemy* hitEnemy = dynamic_cast<Enemy*>(other);
	if (hitEnemy)
	{
		hitEnemy->DealDamage(this, projectileDmg);

		if (!hitEnemy->GetIsInvincible())
			GetGameContext()->GAME_InstantiateEntity<Explosion2>(hitLocation, -6.28f / 3, WE::WVec2(2));
	}

	Destroy();
}

void PlayerProjectile::On_EnterOtherSensor(Entity* otherSensor)
{
	WE::Entity::On_EnterOtherSensor(otherSensor);

	Enemy* hitEnemy = dynamic_cast<Enemy*>(otherSensor);
	if (hitEnemy)
	{
		hitEnemy->DealDamage(this, projectileDmg);

		if (!hitEnemy->GetIsInvincible())
			GetGameContext()->GAME_InstantiateEntity<Explosion2>(GetLocation(), -6.28f / 3, WE::WVec2(2));
	}

	Destroy();
}
