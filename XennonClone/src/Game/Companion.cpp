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
	GetGameContext()->GAME_StartCoroutine(this, CoroutineID::moveToPlayer, moveToPlayerDuration);
	isMovingToPlayer = true;
	canShoot = false;
	isInvincible = true;
	initLocation = GetLocation();
	randOscillationSpeedMult += ((rand() % 20 + 1) / 20.f) * 0.5f - 0.25f;

}

void Companion::HandleMovement(float deltaTime)
{
	if (!GetGameContext()->IsValid(player, playerID))
		Destroy();
	if (!player->HasThisCompanion(this, GetID(), companionSlot))
		Destroy();

	SetLocation(player->GetLocation() + positionOffset + 
		WVec2(sin(randOscillationSpeedMult * 0.7f * GetTimeAlive() + companionSlot) * 1.f, 
			sin(randOscillationSpeedMult * 1.f * GetTimeAlive() - companionSlot) * 0.5f));
}

void Companion::Destroy()
{
	if (!GetGameContext()->IsValid(player, playerID))
		OnRemoveCompanion(this); // to avoid dangling pointers

	SpaceShip::Destroy();
}

void Companion::OnCoroutineUpdate(int ID, float duration)
{
	SpaceShip::OnCoroutineUpdate(ID, duration);

	switch (ID)
	{
	case CoroutineID::moveToPlayer:
	{
		float coroutineProgress = pow(duration / moveToPlayerDuration, 1/2.f);
		SetLocation(
			initLocation * (1 - coroutineProgress) +
			(player->GetLocation() + positionOffset +
				WVec2(sin(randOscillationSpeedMult * 0.7f * GetTimeAlive() + companionSlot) * 1.f,
					sin(randOscillationSpeedMult * 1.f * GetTimeAlive() - companionSlot) * 0.5f)) * coroutineProgress);
	}
	break;
	}
}

void Companion::OnCoroutineEnd(int ID)
{
	SpaceShip::OnCoroutineEnd(ID);

	switch (ID)
	{
	case CoroutineID::moveToPlayer:
		isMovingToPlayer = false;
		canShoot = true;
		isInvincible = false;
		break;
	}
}

