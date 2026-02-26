#include "EnemyDrone.h"
#include <iostream>

#include "PowerUpShield.h"
#include "PowerUpWeapon.h"
#include "PowerUpCompanion.h"

EnemyDrone::EnemyDrone()
{
	filePath = "graphics/drone.bmp";
	hitboxSizeMult = 0.7f;
	hTiles = 8;
	vTiles = 2;
	tileOffset = 0;
	tileSpan = hTiles * vTiles;
	renderLayer = 0;
	animationFPS = 15.f;

	initialSize = WVec2(2.5f);

	bodyDamage = 75.f;

	lifePoints = 25.f;
	moveSpeed = 6.f;
	moveVector = WVec2(0, -1);

	maxLifeTime = 20.f;

	score = 5000;
	initScore = score;

}

void EnemyDrone::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	float oscillator = sin(GetTimeAlive() * 3.14f);
	float sign = std::copysign(1.0f, oscillator);
	float sideWaysForce = sign * pow(abs(oscillator), 1/1.2f);
	
	Move(moveVector * moveSpeed + WVec2(sideWaysForce, 0) * moveSpeed*1.5f);
}

void EnemyDrone::DieByPlayer()
{
	score *= ++deathCountInGroup;

	if (deathCountInGroup + 1 == 5)
		GetGameContext()->GAME_InstantiateEntity<PowerUpShield>(GetLocation(), 0.f);
	else if (deathCountInGroup + 1 == 10)
		GetGameContext()->GAME_InstantiateEntity<PowerUpWeapon>(GetLocation(), 0.f);
	//else if (deathCountInGroup + 1 == 12)
		//GetGameContext()->GAME_InstantiateEntity<PowerUpCompanion>(GetLocation(), -3.141592/2);

	Enemy::DieByPlayer();
}

void EnemyDrone::Destroy()
{
	EnemyDrone* nextDrone = nextDroneInGroup;
	uint32_t nextID = nextDroneInGroupID;
	while (true)
	{
		if (!GetGameContext()->IsValid(nextDrone, nextID))
			break;
		nextDrone->deathCountInGroup = deathCountInGroup;
		nextID = nextDrone->nextDroneInGroupID;
		nextDrone = nextDrone->nextDroneInGroup;
	}
	EnemyDrone* prevDrone = previousDroneInGroup;
	uint32_t prevID = previousDroneInGroupID;
	while (true)
	{
		if (!GetGameContext()->IsValid(prevDrone, prevID))
			break;
		prevDrone->deathCountInGroup = deathCountInGroup;
		prevID = prevDrone->previousDroneInGroupID;
		prevDrone = prevDrone->previousDroneInGroup;
	}

	if (GetGameContext()->IsValid(nextDroneInGroup, nextDroneInGroupID))
	{
		nextDroneInGroup->previousDroneInGroup = previousDroneInGroup;
		nextDroneInGroup->previousDroneInGroupID = previousDroneInGroupID;
	}
		
	if (GetGameContext()->IsValid(previousDroneInGroup, previousDroneInGroupID))
	{
		previousDroneInGroup->nextDroneInGroup = nextDroneInGroup;
		previousDroneInGroup->nextDroneInGroupID = nextDroneInGroupID;
	}

	Entity::Destroy();
}