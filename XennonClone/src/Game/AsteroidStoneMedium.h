#pragma once
#include "Asteroid.h"
#include "AsteroidStoneSmall.h"

class AsteroidStoneMedium : public Asteroid
{
public:
	AsteroidStoneMedium()
	{
		filePath = "graphics/SAster64.bmp";
		hitboxSizeMult = 0.8f;
		hTiles = 8;
		vTiles = 3;
		tileOffset = 0;
		tileSpan = hTiles * vTiles;
		renderLayer = 0;
		animationFPS = 9.f;

		lifePoints = 25.f;

		moveSpeed = 0.f;

		score = 15000;
	}

	void DieByPlayer() override
	{
		//spawn 3 small rocks
		WVec2 initMoveVector = moveVector * moveSpeed;

		for (int i = 0; i < 3; ++i)
		{

			WVec2 moveVector; //random unit vector
			float randomRotation = GetRotation() + (6.28 / 3) * i;
			moveVector.x = cos(randomRotation);
			moveVector.y = sin(randomRotation);

			float randomSpeedMult = (rand() % 20 + 1) / 20.f; // 0+ to 1
			float maxRandomness = 0.4f; // must be less than 1.f
			randomSpeedMult = 1.f - (maxRandomness / 2) + (randomSpeedMult * maxRandomness); //range from 0.8 to 1.2

			float asteroidSpeed = 1.5f; // base speed

			moveVector *= asteroidSpeed * randomSpeedMult;
			moveVector += initMoveVector; // add original asteroid speed to new asteroid to keep momentum

			auto enemy = GetGameContext()->GAME_InstantiateEntity<AsteroidStoneSmall>(GetLocation(), GetRotation() + (6.28 / 3) * i, WE::WVec2(2.f));

			if (!enemy)
				return;

			GetGameContext()->RENDER_SetAnimationParameters(enemy, true, enemy->GetAnimFps() * randomSpeedMult * 2);
			enemy->moveVector = moveVector;
			enemy->moveSpeed = 1.f;
			enemy->maxLifeTime = 40.f;
			enemy->score = 7500;
			if (GetLevelInstance())
				enemy->SetLevelInstance(GetLevelInstance());

		}

		Enemy::DieByPlayer();
	}
};

