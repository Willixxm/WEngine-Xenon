#include "PowerUp.h"
#include "SpaceShip.h"
#include "Companion.h"
#include "TextRenderer.h"
#include <iostream>

PowerUp::PowerUp()
{
	initialSize = WE::WVec2(2.f);
}

void PowerUp::Start()
{
	Pawn::Start();

	GetGameContext()->PHYS_AddPhysComponentToEntity(this, WE::WBodyType::kinematicBody, GetInitialSize() * hitboxSizeMult, collisionLayer, collidesWith, isSensor, 1.f);
	GetGameContext()->RENDER_AddRenderComponent(this, filePath, hTiles, vTiles, tileOffset, tileSpan, renderLayer);
	GetGameContext()->RENDER_SetAnimationParameters(this, true, animationFPS);
}

void PowerUp::Update(float deltaTime)
{
	Pawn::Update(deltaTime);

	if (GetTimeAlive() > maxLifeTime)
	{
		Destroy();
	}
	
	Move(WVec2(-1.f, 0) * moveSpeed);
}

void PowerUp::On_SensorBeginOverlap(Entity* other)
{
	Entity::On_SensorBeginOverlap(other);

	if (!other)
		return;

	SpaceShip* hitPlayer = dynamic_cast<SpaceShip*>(other);
	if (hitPlayer)
	{
		if (hasBeenPickedUp)
			return;
		if (OnPickUp(hitPlayer))
		{
			hasBeenPickedUp = true;
			TextRenderer* text = GetGameContext()->GAME_InstantiateEntity<TextRenderer>(GetLocation() + WVec2(0, 2), 0.f, WVec2(0.6));
			text->SetFont(false);
			text->SetText(pickUpText);
			text->DestroyWithFloat(WVec2(0, 2), 1.5f);
			Destroy();
		}
	}
}

bool PowerUp::OnPickUp(SpaceShip* ship)
{
	return true;
}