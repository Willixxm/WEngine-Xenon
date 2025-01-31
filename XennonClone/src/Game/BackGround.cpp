#include "BackGround.h"
#include "WEngine/GameContext.h"
#include "WEngine/WMath.h"

void BackGround::Start()
{
	WE::Entity::Start();

	GetGameContext()->RENDER_AddRenderComponent(this, filePath, 1, 1, 0, 1*1, layer, WE::WVec2(0));
	GetGameContext()->RENDER_SetAnimationParameters(this, false, 0.f);

}

void BackGround::Update(float deltaTime)
{
	WE::Entity::Update(deltaTime);

	WE::WVec2 pos = GetLocation();
	if (pos.y < -40)
	{
		GetGameContext()->PHYS_SetLocationOnPhysObj(this->bodyId, WE::WVec2(pos.x, pos.y + 80));
	}
}