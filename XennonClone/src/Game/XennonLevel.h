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

		GetGameContext()->RENDER_SetOrthoCameraSize(35.f);

		SpaceShip* playerShip = GetGameContext()->GAME_InstantiateEntity<SpaceShip>(WE::WVec2(0, -10), WE::WVec2(4));

		BackGround* background = GetGameContext()->GAME_InstantiateEntity<BackGround>(WE::WVec2(0, 0), WE::WVec2(0));
		ParallaxBackground* parallaxBackground = GetGameContext()->GAME_InstantiateEntity<ParallaxBackground>(WE::WVec2(0, 0), WE::WVec2(0));


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

			if (num < 25)
			{
				Enemy* loner2 = GetGameContext()->GAME_InstantiateEntity<EnemyLoner>(WE::WVec2(sign * 16, 11), WE::WVec2(4));
			}
			else if (num < 50)
			{
				Enemy* loner1 = GetGameContext()->GAME_InstantiateEntity<EnemyLoner>(WE::WVec2(sign * 15, 10), WE::WVec2(4));
				Enemy* loner2 = GetGameContext()->GAME_InstantiateEntity<EnemyLoner>(WE::WVec2(sign * 19, 20), WE::WVec2(4));
			}
			else if (num < 75)
			{
				Enemy* rusher1 = GetGameContext()->GAME_InstantiateEntity<EnemyRusher>(WE::WVec2(sign * 0, 20), WE::WVec2(4));
				Enemy* rusher2 = GetGameContext()->GAME_InstantiateEntity<EnemyRusher>(WE::WVec2(sign * 2, 25), WE::WVec2(4));
				Enemy* rusher3 = GetGameContext()->GAME_InstantiateEntity<EnemyRusher>(WE::WVec2(sign * -1, 30), WE::WVec2(4));
				Enemy* rusher4 = GetGameContext()->GAME_InstantiateEntity<EnemyRusher>(WE::WVec2(sign * 3, 35), WE::WVec2(4));
			}
			else
			{
				Enemy* rusher1 = GetGameContext()->GAME_InstantiateEntity<EnemyRusher>(WE::WVec2(sign * 8, 20), WE::WVec2(4));
				Enemy* rusher2 = GetGameContext()->GAME_InstantiateEntity<EnemyRusher>(WE::WVec2(sign * 10, 25), WE::WVec2(4));
			}


		}


	}


};