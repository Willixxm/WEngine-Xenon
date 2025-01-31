#include "PlayerProjectile.h"
#include "WEngine/Entity.h"
#include "WEngine/WMath.h"
#include "WEngine/GameContext.h"
#include "Enemy.h"
#include "Explosion.h"


void PlayerProjectile::Start()
{
	WE::Entity::Start();

	GetGameContext()->PHYS_AddPhysComponentToEntity(this, WE::WBodyType::kinematicBody, GetInitialSize());
	GetGameContext()->PHYS_SetLinearVelocityOnPhysObj(this->bodyId, WE::WVec2(0, projectileSpeed));
	GetGameContext()->RENDER_AddRenderComponent(this, "graphics/missile.bmp", 2, 3, 0, 2, 1);
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

void PlayerProjectile::On_CollisionBegin(WE::Entity* entity, WE::WVec2 hitLocation)
{
	WE::Entity::On_CollisionBegin(entity, hitLocation);

	
	Enemy* hitEnemy = dynamic_cast<Enemy*>(entity);
	if (hitEnemy)
	{
		hitEnemy->DealDamage(projectileDmg);

		GetGameContext()->GAME_InstantiateEntity<Explosion>(hitLocation, WE::WVec2(2));

		Destroy();
	}
}