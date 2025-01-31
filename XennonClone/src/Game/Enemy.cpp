#include "Enemy.h"
#include "WEngine/GameContext.h"


using namespace WE;

void Enemy::Start()
{
	Pawn::Start();

	GetGameContext()->PHYS_AddPhysComponentToEntity(this, WE::WBodyType::dynamicBody, GetInitialSize());
	GetGameContext()->PHYS_SetLinearVelocityOnPhysObj(this->bodyId, WE::WVec2(1, 0));
	GetGameContext()->RENDER_AddRenderComponent(this, "graphics/LonerA.bmp", 4, 4, 0, 16, 0);
	GetGameContext()->RENDER_SetAnimationParameters(this, true, 5.f);

}


void Enemy::Update(float deltaTime)
{
	Pawn::Update(deltaTime);







}