#include "Explosion.h"
#include "WEngine/Entity.h"
#include "WEngine/GameContext.h"

using namespace WE;

void Explosion::Start()
{
	Entity::Start();

	GetGameContext()->RENDER_AddRenderComponent(this, filePath, hTiles, vTiles, 0, hTiles * vTiles, GetInitialSize().x / 2);
	
	lifeTime = (GetInitialSize().x + GetInitialSize().y)/2 * lifeTimeMult;
	
	GetGameContext()->RENDER_SetAnimationParameters(this, true, (hTiles * vTiles) / lifeTime);

}


void Explosion::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	if (GetTimeAlive() > lifeTime)
	{
		Destroy();
	}

}