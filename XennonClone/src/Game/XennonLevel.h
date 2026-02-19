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

		GetGameContext()->RENDER_SetOrthoCameraSize(34.f * 1);

		SpaceShip* playerShip = GetGameContext()->GAME_InstantiateEntity<SpaceShip>(WE::WVec2(-20, 0), 2*3.14159265 * -1/4, WE::WVec2(4));

		BackGround* background = GetGameContext()->GAME_InstantiateEntity<BackGround>(WE::WVec2(0, 0), 0.f, WE::WVec2(0));
		
		int parallaxBackgroundScale = 2; //size of each tile
		ParallaxBackground* parallaxBackground = GetGameContext()->GAME_InstantiateEntity<ParallaxBackground>(
			WE::WVec2(0), 
			2 * 3.14159265 * -1 / 4, 
			WE::WVec2(parallaxBackgroundScale));
	}

private:
	float spawnCoolDown = 2.5f;
	float timeUntilNextSpawn = spawnCoolDown;


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
			int num = rand() % 100 + 1;
			int sign = (num % 2 ? -1 : 1);

			/*
			if (num < 25)
			{
				Enemy* loner2 = GetGameContext()->GAME_InstantiateEntity<EnemyLoner>(WE::WVec2(sign * 16, 11), 0.f, WE::WVec2(4));
			}
			else if (num < 50)
			{
				Enemy* loner1 = GetGameContext()->GAME_InstantiateEntity<EnemyLoner>(WE::WVec2(sign * 15, 10), 0.f, WE::WVec2(4));
				Enemy* loner2 = GetGameContext()->GAME_InstantiateEntity<EnemyLoner>(WE::WVec2(sign * 19, 20), 0.f, WE::WVec2(4));
			}
			else if (num < 75)
			{
				Enemy* rusher1 = GetGameContext()->GAME_InstantiateEntity<EnemyRusher>(WE::WVec2(sign * 0, 20), 0.f, WE::WVec2(4));
				Enemy* rusher2 = GetGameContext()->GAME_InstantiateEntity<EnemyRusher>(WE::WVec2(sign * 2, 25), 0.f, WE::WVec2(4));
				Enemy* rusher3 = GetGameContext()->GAME_InstantiateEntity<EnemyRusher>(WE::WVec2(sign * -1, 30), 0.f, WE::WVec2(4));
				Enemy* rusher4 = GetGameContext()->GAME_InstantiateEntity<EnemyRusher>(WE::WVec2(sign * 3, 35), 0.f, WE::WVec2(4));
			}
			else
			{
				Enemy* rusher1 = GetGameContext()->GAME_InstantiateEntity<EnemyRusher>(WE::WVec2(sign * 8, 20), 0.f, WE::WVec2(4));
				Enemy* rusher2 = GetGameContext()->GAME_InstantiateEntity<EnemyRusher>(WE::WVec2(sign * 10, 25), 0.f, WE::WVec2(4));
			}
			*/

		}


	}


};