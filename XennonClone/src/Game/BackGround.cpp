#include "BackGround.h"
#include "WEngine/GameContext.h"
#include "WEngine/WMath.h"

void BackGround::Start()
{
	WE::Entity::Start();

	GetGameContext()->RENDER_AddRenderComponent(this, filePath, 1, 1, 50, 40, WE::WRenderType::Render_Surface, layer);
	GetGameContext()->RENDER_SetAnimationParameters(this, false, 0.f);
	GetGameContext()->PHYS_AddPhysComponentToEntity(this, WE::WBodyType::kinematicBody, WE::WVec2(0, 0));
	GetGameContext()->PHYS_SetLinearVelocityOnPhysObj(this->bodyId, WE::WVec2(0, scrollVel));
}

void BackGround::Update(float deltaTime)
{
	WE::Entity::Update(deltaTime);

	WE::WVec2 pos = GetGameContext()->PHYS_GetLocationOfPhysObj(this->bodyId);
	if (pos.y < -40)
	{
		GetGameContext()->PHYS_SetLocationOnPhysObj(this->bodyId, WE::WVec2(pos.x, pos.y + 80));
	}
}