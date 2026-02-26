#include "Companion.h"
#include "WEngine/GameContext.h"
#include "Explosion.h"
#include "SpaceShip.h"

#include <iostream>

using namespace WE;

Companion::Companion()
{
	filePath = "graphics/clone.bmp";

	hTiles = 4;
	vTiles = 5;
	tileOffset = 0;
	tileSpan = 16;

	flashTileOffset = 16;
	flashTileSpan = 1;

	maxHealth = 50.f;
	currentHealth = maxHealth;

	extraLives = 0;

}

void Companion::Start()
{
	Pawn::Start();

	GetGameContext()->PHYS_AddPhysComponentToEntity(this, WBodyType::staticBody, GetInitialSize() * 0.75f, WCollisionLayer::Layer1, WCollisionLayer::Layer3, false, 0.f);
	GetGameContext()->RENDER_AddRenderComponent(this, filePath, hTiles, vTiles, tileOffset, tileSpan, layer);
	GetGameContext()->RENDER_SetAnimationParameters(this, true, 20.f);

}

void Companion::SetPlayer(SpaceShip* player_, uint32_t playerID_, int compSlot)
{
	player = player_;
	playerID = playerID_;
	companionSlot = compSlot;
	switch (companionSlot)
	{
	default:
		std::cout << "invalid companion slot!! =------------------\n";
	case 0:
		positionOffset = WVec2(0, -5);
		break;
	case 1:
		positionOffset = WVec2(0, 5);
		break;
	}

}

void Companion::HandleMovement(float deltaTime)
{
	if (!GetGameContext()->IsValid(player, playerID))
		Destroy();
	if (!player->HasThisCompanion(this, GetID(), companionSlot))
		Destroy();

	SetLocation(player->GetLocation() + positionOffset);
}

void Companion::Destroy()
{
	if (!GetGameContext()->IsValid(player, playerID))
		OnRemoveCompanion(this); // to avoid dangling pointers

	SpaceShip::Destroy();
}
