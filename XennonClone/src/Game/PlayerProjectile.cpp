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

	GetGameContext()->PHYS_AddPhysComponentToEntity(this, WE::WBodyType::dynamicBody, GetInitialSize(), WCollisionLayer::Layer2, WCollisionLayer::Layer3, false, 1.f);
	GetGameContext()->PHYS_SetLinearVelocityOnPhysObj(this->bodyId, GetUpVector() * projectileSpeed);
	GetGameContext()->RENDER_AddRenderComponent(this, filePath, hTiles, vTiles, tileOffset, tileSpan, layer);
	GetGameContext()->RENDER_SetAnimationParameters(this, true, animFps);


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

		//if (!hitEnemy->GetIsInvincible())
			OnDestroy(hitLocation);
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

		//if (!hitEnemy->GetIsInvincible())
			OnDestroy(GetLocation());
	}

	Destroy();
}

void PlayerProjectile::OnDestroy(WVec2 location)
{
	GetGameContext()->GAME_InstantiateEntity<Explosion2>(location, -6.28f / 3, WE::WVec2(2));
}
