#include "PlayerProjectile.h"
#include "WEngine/Entity.h"
#include "WEngine/WMath.h"
#include "WEngine/GameContext.h"
#include "Enemy.h"
#include "Explosion.h"


void PlayerProjectile::Start()
{
	WE::Entity::Start();

	GetGameContext()->PHYS_AddPhysComponentToEntity(this, WE::WBodyType::kinematicBody, GetInitialSize(), WCollisionLayer::Layer2, WCollisionLayer::Layer3, true);
	GetGameContext()->PHYS_SetLinearVelocityOnPhysObj(this->bodyId, WE::WVec2(0, projectileSpeed));
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

void PlayerProjectile::On_SensorBeginOverlap(Entity* other)
{
	WE::Entity::On_SensorBeginOverlap(other);

	
	Enemy* hitEnemy = dynamic_cast<Enemy*>(other);
	if (hitEnemy)
	{
		hitEnemy->DealDamage(projectileDmg);

		GetGameContext()->GAME_InstantiateEntity<Explosion>(GetLocation(), WE::WVec2(2));

	}

	Destroy();
}
