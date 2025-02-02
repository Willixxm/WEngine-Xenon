#include "Enemy.h"
#include "WEngine/GameContext.h"
#include "WEngine/Pawn.h"
#include "PlayerProjectile.h"
#include "Explosion.h"



using namespace WE;

void Enemy::Start()
{
	Pawn::Start();

	GetGameContext()->PHYS_AddPhysComponentToEntity(this, WE::WBodyType::dynamicBody, GetInitialSize() * hitboxSizeMult, WCollisionLayer::Layer3, WCollisionLayer::Layer1 | WCollisionLayer::Layer2, false);
	GetGameContext()->PHYS_SetLinearVelocityOnPhysObj(this->bodyId, WE::WVec2(0, -2));
	GetGameContext()->RENDER_AddRenderComponent(this, "graphics/LonerA.bmp", 4, 4, 0, 16, 0);
	GetGameContext()->RENDER_SetAnimationParameters(this, true, 5.f);

}


void Enemy::Update(float deltaTime)
{
	Pawn::Update(deltaTime);






}


void Enemy::DealDamage(float damage)
{
	Pawn::DealDamage(damage);

	lifePoints -= damage;

	if (lifePoints <= 0)
	{
		GetGameContext()->GAME_InstantiateEntity<Explosion>(GetLocation(), WE::WVec2(5));

		Destroy();
	}


	
}