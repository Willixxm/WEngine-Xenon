#pragma once

#include "WEngine/Entity.h"
#include "WEngine/GameContext.h"

#include "SpaceShip.h"
#include "BackGround.h"
#include "ParallaxBackground.h"
#include "Enemy.h"
#include "EnemyLoner.h"
#include "EnemyRusher.h"

#include <cstdlib>
#include <cmath>


class XennonLevel : public WE::Entity
{

public:
	void Start() override
	{
		WE::Entity::Start();

		GetGameContext()->MEMORY_SetAutoUnloadAssetIfUnused(false);

		GetGameContext()->RENDER_SetOrthoCameraSize(34.f * 2);

		player = GetGameContext()->GAME_InstantiateEntity<SpaceShip>(WE::WVec2(-20, 0), 2*3.14159265 * -1/4, WE::WVec2(4));

		BackGround* background = GetGameContext()->GAME_InstantiateEntity<BackGround>(WE::WVec2(0, 0), 0.f, WE::WVec2(0));
		
		int parallaxBackgroundScale = 2; //size of each tile
		ParallaxBackground* parallaxBackground = GetGameContext()->GAME_InstantiateEntity<ParallaxBackground>(
			WE::WVec2(0), 
			2 * 3.14159265 * -1 / 4, 
			WE::WVec2(parallaxBackgroundScale));
	}

public:
	Entity* GetPlayer() const { return player; }
private:
	Entity* player = nullptr;









private:
	float spawnCoolDown = 2.5f;
	float timeUntilNextSpawn = spawnCoolDown;

	WVec2 playSize = WVec2(60.f, 30.f);
	float padding = 1.2f;

	float rndPercent()	{
		return (rand() % 101) / 100.f;
	}
	int rndSign() {
		return (rand() % 2 ? -1 : 1);
	}
	WVec2 rndPlaceOnBoard()	{
		return WVec2(
			playSize.x * rndPercent() - playSize.x / 2,
			playSize.y * rndPercent() - playSize.y / 2);
	}

	void Spawn_Loner() {
		WVec2 spawnLocation = rndPlaceOnBoard();
		spawnLocation.x *= 0.25f;
		spawnLocation.x += playSize.x;

		EnemyLoner* enemy = GetGameContext()->GAME_InstantiateEntity<EnemyLoner>(spawnLocation, 0.f, WE::WVec2(4));
		enemy->SetLevelInstance(this);
	}
	void Spawn_Rusher()
	{
		WVec2 spawnLocation = rndPlaceOnBoard();
		int sign = rndSign();
		spawnLocation.y += playSize.y * sign;
		EnemyRusher* enemy = GetGameContext()->GAME_InstantiateEntity<EnemyRusher>(spawnLocation, 0.f, WE::WVec2(4));
		enemy->SetLevelInstance(this);
		enemy->normalizedMoveVector *= sign;
		
	}
	void Spawn_DroneSwarm(int count, float spawnRatePerSecond)
	{

	}

public:
	void Update(float deltaTime) override
	{

		WE::Entity::Update(deltaTime);

		timeUntilNextSpawn -= deltaTime;

		if (timeUntilNextSpawn <= 0)
		{
			timeUntilNextSpawn = spawnCoolDown;
			if (spawnCoolDown > 1.f)
				spawnCoolDown *= 0.98f;

			int spawnChance = rand() % 100 + 1;

			int sign = 1;

			if (spawnChance < 25)
			{
				Spawn_Loner();
				Spawn_Loner();
			}
			else if (spawnChance < 50)
			{
				Spawn_Rusher();
				Spawn_Rusher();
				Spawn_Rusher();
			}
			else if (spawnChance < 75)
			{
				Spawn_Rusher();
				Spawn_Rusher();
			}
			else // 
			{
				Spawn_Loner();
			}
			

		}


	}


};