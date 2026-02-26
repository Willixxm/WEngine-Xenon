#pragma once

#include "WEngine/Entity.h"
#include "WEngine/GameContext.h"

#include "SpaceShip.h"
#include "LifeCountDisplay.h"
#include "TextRenderer.h"
#include "BackGround.h"
#include "ParallaxBackground.h"
#include "Enemy.h"
#include "EnemyLoner.h"
#include "EnemyRusher.h"
#include "DroneSpawner.h"

#include "AsteroidStoneBig.h"
#include "AsteroidStoneMedium.h"
#include "AsteroidStoneSmall.h"
#include "AsteroidMetalBig.h"
#include "AsteroidMetalMedium.h"
#include "AsteroidMetalSmall.h"

#include "Companion.h"

#include <cstdlib>
#include <cmath>
#include <iostream>

#include <string>
#include <iomanip>
#include <sstream>

class XennonLevel : public WE::Entity
{

public:
	void Start() override
	{
		WE::Entity::Start();

		GetGameContext()->MEMORY_SetAutoUnloadAssetIfUnused(false);

		float camSize = 34.f * 1.f;
		GetGameContext()->RENDER_SetOrthoCameraSize(camSize);

		player = GetGameContext()->GAME_InstantiateEntity<SpaceShip>(WE::WVec2(-20, 0), 2 * 3.14159265 * -1 / 4, WE::WVec2(4));
		playerID = player->GetID();

		// GAME BORDERS
		Entity* topBorder = GetGameContext()->GAME_InstantiateEntity<Entity>(WE::WVec2(0, 19), 0, WE::WVec2(100, 2));
		GetGameContext()->PHYS_AddPhysComponentToEntity(topBorder, WE::WBodyType::staticBody, topBorder->GetInitialSize(), WCollisionLayer::Layer3, WCollisionLayer::Layer1, false, 1.f);
		Entity* bottomBorder = GetGameContext()->GAME_InstantiateEntity<Entity>(WE::WVec2(0, -19), 0, WE::WVec2(100, 2));
		GetGameContext()->PHYS_AddPhysComponentToEntity(bottomBorder, WE::WBodyType::staticBody, topBorder->GetInitialSize(), WCollisionLayer::Layer3, WCollisionLayer::Layer1, false, 1.f);
		Entity* rightBorder = GetGameContext()->GAME_InstantiateEntity<Entity>(WE::WVec2(40, 0), 0, WE::WVec2(2, 30));
		GetGameContext()->PHYS_AddPhysComponentToEntity(rightBorder, WE::WBodyType::staticBody, rightBorder->GetInitialSize(), WCollisionLayer::Layer3, WCollisionLayer::Layer1 | WCollisionLayer::Layer2, false, 1.f);
		//Entity* leftBorder = GetGameContext()->GAME_InstantiateEntity<Entity>(WE::WVec2(-40, 0), 0, WE::WVec2(2, 30));
		//GetGameContext()->PHYS_AddPhysComponentToEntity(leftBorder, WE::WBodyType::staticBody, leftBorder->GetInitialSize(), WCollisionLayer::Layer3, WCollisionLayer::Layer1, false, 1.f);
		
		GetGameContext()->RENDER_AddRenderComponent(topBorder, "graphics/bblogo.bmp", 20);
		GetGameContext()->RENDER_AddRenderComponent(bottomBorder, "graphics/bblogo.bmp", 20);
		GetGameContext()->RENDER_AddRenderComponent(rightBorder, "graphics/bblogo.bmp", 20);
		//GetGameContext()->RENDER_AddRenderComponent(leftBorder, "graphics/bblogo.bmp", 20);
	


		float aspRatio = 16.f / 7;
		float safeBorder = camSize * 0.03f; //safe border

		// ============
		// PLAYER SCORE
		auto playerNameText = GetGameContext()->GAME_InstantiateEntity<TextRenderer>(
			WE::WVec2(aspRatio * (-camSize / 2) + safeBorder, (camSize / 2) - safeBorder)
			, 0.f, WE::WVec2(0.8f));
		playerNameText->SetText("Player One");

		scoreText = GetGameContext()->GAME_InstantiateEntity<TextRenderer>(
			WE::WVec2(aspRatio * (-camSize / 2) + safeBorder + 0.3f, (camSize / 2) - safeBorder*2 - 0.2f)
			, 0.f, WE::WVec2(1.5f));
		UpdateScoreText();
		// ============


		// ==========
		// HIGH SCORE
		auto highScoreTitle = GetGameContext()->GAME_InstantiateEntity<TextRenderer>(
			WE::WVec2(0, (camSize / 2) - safeBorder)
			, 0.f, WE::WVec2(0.7f));
		highScoreTitle->textCentered = true;
		highScoreTitle->SetFont(false);
		highScoreTitle->SetText("Hi Score");

		hiScoreText = GetGameContext()->GAME_InstantiateEntity<TextRenderer>(
			WE::WVec2(0, (camSize / 2) - safeBorder * 2 + 0.2f)
			, 0.f, WE::WVec2(0.7f));
		hiScoreText->textCentered = true;
		hiScoreText->SetFont(false);
		UpdateHiScoreText();
		// ==========


		// ==========
		// HEALTH BAR
		Entity* healthBar = GetGameContext()->GAME_InstantiateEntity<Entity>(
			WE::WVec2(aspRatio * (-camSize / 2) + safeBorder + 5, (-camSize / 2) + safeBorder + 2)
			, 0.f, WE::WVec2(10));
		GetGameContext()->RENDER_AddRenderComponent(healthBar, "graphics/healthBar.bmp", 1, 60, 0, 60, 21);
		GetGameContext()->RENDER_SetAnimationParameters(healthBar, false);
		if (player)
		{
			GetGameContext()->RENDER_SetManualAnimationState(healthBar, (player->GetMaxHealth() - player->GetCurrentHealth()) / player->GetMaxHealth());
			player->healthBar = healthBar;
			player->healthBarID = healthBar->GetID();
		}
		// ==========

		// ==========
		// Extra life Display
		LifeCountDisplay* extraLifeDisplay = GetGameContext()->GAME_InstantiateEntity<LifeCountDisplay>(
			WE::WVec2(aspRatio * (-camSize / 2) + safeBorder + 1.5, (-camSize / 2) + safeBorder + 4)
			, 0.f, WE::WVec2(2));
		if (player)
		{
			player->extraLifeCounter = extraLifeDisplay;
			player->extraLifeCounterID = extraLifeDisplay->GetID();
			extraLifeDisplay->SetExtraLifeCount(player->extraLives);
		}

		BackGround* background = GetGameContext()->GAME_InstantiateEntity<BackGround>(WE::WVec2(0, 0), 0.f, WE::WVec2(0));
		
		int parallaxBackgroundScale = 2; //size of each tile
		ParallaxBackground* parallaxBackground = GetGameContext()->GAME_InstantiateEntity<ParallaxBackground>(
			WE::WVec2(0), 
			2 * 3.14159265 * -1 / 4, 
			WE::WVec2(parallaxBackgroundScale));
	}

public:
	Entity* GetPlayer()
	{
		if (GetGameContext()->IsValid(player, playerID))
			return player;
		else
			return nullptr;
	}
private:
	SpaceShip* player = nullptr;
	uint32_t playerID = 0;
	TextRenderer* scoreText = nullptr;
	unsigned int playerScore;

	TextRenderer* hiScoreText = nullptr;
	unsigned int playerHiScore = 521533;

	void UpdateScoreText()
	{
		std::ostringstream oss;
		oss << std::setw(10) << std::setfill('0') << playerScore;

		scoreText->SetText(oss.str());

		if (playerScore > playerHiScore)
		{
			playerHiScore = playerScore;
			UpdateHiScoreText();
		}
	}
	void UpdateHiScoreText()
	{
		std::ostringstream oss;
		oss << std::setw(10) << std::setfill('0') << playerHiScore;

		hiScoreText->SetText(oss.str());
	}

public:
	void AddScore(int score)
	{
		playerScore += score;
		UpdateScoreText();
	}


public:
	float difficulty = 0.6f; //set from 0 to 1


private:
	float spawnCoolDown = 4.0f;
	float minimumSpawnCoolDown = 2.5f;
	float spawnCoolDownMultiplier = 0.992f;

	float timeUntilNextSpawn = 0;

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



	// ====================
	// === SPAWNING =======
	// ====================

	void Spawn_Loner() {
		WVec2 spawnLocation = rndPlaceOnBoard();
		spawnLocation.x *= 0.5f;
		spawnLocation.x += playSize.x;

		EnemyLoner* enemy = GetGameContext()->GAME_InstantiateEntity<EnemyLoner>(spawnLocation, 0.f);
		enemy->SetLevelInstance(this);
	}

	void Spawn_Rusher()
	{
		WVec2 spawnLocation = rndPlaceOnBoard();
		spawnLocation.x *= 0.5f;
		spawnLocation.x += playSize.x/3;
		int sign = rndSign();
		spawnLocation.y += playSize.y * 1.2f * sign;
		EnemyRusher* enemy = GetGameContext()->GAME_InstantiateEntity<EnemyRusher>(spawnLocation, 0.f);
		enemy->SetLevelInstance(this);
		enemy->moveVector = WVec2(0, -sign);
		
	}

	void Spawn_DroneSwarm(int count, float spawnInterval)
	{
		WVec2 spawnLocation = rndPlaceOnBoard();
		int sign = rndSign();
		spawnLocation.y += playSize.y * 1.2f * sign;
		DroneSpawner* spawner = GetGameContext()->GAME_InstantiateEntity<DroneSpawner>(spawnLocation, 0.f);
		spawner->SetLevelInstance(this);
		spawner->moveVector = WVec2(0, -sign);
		spawner->enemiesToSpawn = count;
		spawner->spawnInterval = spawnInterval;
		spawner->StartSpawning();
	}

	void Spawn_Asteroid(int size_division, bool metal)
	{
		WVec2 spawnLocation = rndPlaceOnBoard();
		spawnLocation.x *= 0.5f;
		spawnLocation.x += playSize.x;

		Enemy* enemy = nullptr;

		float randomRotation = (rand() % 618) * 0.01f;

		if (metal)
		{
			switch (size_division)
			{
			case 2:
				enemy = GetGameContext()->GAME_InstantiateEntity<AsteroidMetalSmall>(spawnLocation, randomRotation);
				break;
			case 1:
				enemy = GetGameContext()->GAME_InstantiateEntity<AsteroidMetalMedium>(spawnLocation, randomRotation);
				break;
			case 0:
			default:
				enemy = GetGameContext()->GAME_InstantiateEntity<AsteroidMetalBig>(spawnLocation, randomRotation);
				break;
			}
		}
		else
		{
			switch (size_division)
			{
			case 2:
				enemy = GetGameContext()->GAME_InstantiateEntity<AsteroidStoneSmall>(spawnLocation, randomRotation);
				break;
			case 1:
				enemy = GetGameContext()->GAME_InstantiateEntity<AsteroidStoneMedium>(spawnLocation, randomRotation);
				break;
			case 0:
			default:
				enemy = GetGameContext()->GAME_InstantiateEntity<AsteroidStoneBig>(spawnLocation, randomRotation);
				break;
			}			 
		}

		
		if (!enemy)
			return;

		float moveSpeed = 3.f;

		float randomVariance = ((rand() % 100) / 100.f); // random float 0 to 1
		moveSpeed += randomVariance * (moveSpeed / 2); //speed up to 150%
		GetGameContext()->RENDER_SetAnimationParameters(enemy, true, enemy->GetAnimFps() * randomVariance * 2);
		enemy->moveVector = WVec2(-1, ((rand() % 100) / 100.f - 50.f) * (1 / 10)).normalized();
		enemy->moveSpeed = moveSpeed;
		enemy->SetLevelInstance(this);
		enemy->maxLifeTime = 40.f;
	}

public:
	void Update(float deltaTime) override
	{

		WE::Entity::Update(deltaTime);

		timeUntilNextSpawn -= deltaTime;

		if (timeUntilNextSpawn <= 0 && true)
		{
			timeUntilNextSpawn = spawnCoolDown;
			if (spawnCoolDown > minimumSpawnCoolDown)
				spawnCoolDown *= spawnCoolDownMultiplier;

			if (difficulty < 1)
				difficulty += 0.01f;

			float spawnDifficulty = (rand() % 100 + 1) / 100.f; // 0.01 to 1 random number
			
			//std::cout << "random value: " << spawnDifficulty << '\n';

			float diff;
			if (difficulty < 0.01f)
				diff = 0.01f;
			else if (difficulty > 1.f)
				diff = 1.f;
			else
				diff = difficulty;

			//std::cout << "difficulty: " << difficulty << '\n';
			//std::cout << "corrected diff: " << diff << '\n';

			diff = pow(1.f - diff, 2.5f) + 0.5f; //transform range from 0 to 1 linear into 0.5 to 1.5 exponential, inverted

			//std::cout << "taper: " << diff << '\n';

			float n = pow(spawnDifficulty, pow(diff, 2));
			float d = (2 * diff);

			spawnDifficulty = n / d; //bias random spawnDifficulty using difficulty value

			//std::cout << "wave difficulty: " << spawnDifficulty << '\n';
			//std::cout << "===================" << '\n';

			

			// spawn waves from hardest->easiest
			if (spawnDifficulty > 0.9f) 
			{
				Spawn_Loner();
				Spawn_Loner();
				
				Spawn_Asteroid(0, true);
				Spawn_Asteroid(0, false);

				Spawn_DroneSwarm(12, 0.5f);

				Spawn_Asteroid(2, true);
			}
			else if (spawnDifficulty > 0.8f)
			{
				Spawn_Rusher();
				Spawn_Rusher();
				Spawn_Rusher();
				Spawn_Rusher();
				Spawn_Rusher();

				Spawn_Asteroid(0, rand() % 2);
				Spawn_Asteroid(rand() % 3, rand() % 2);
				Spawn_Asteroid(rand() % 3, rand() % 2);
			}
			else if (spawnDifficulty > 0.7f)
			{
				Spawn_Loner();
				Spawn_Loner(); 
				Spawn_Loner();
				Spawn_Loner();
			}
			else if (spawnDifficulty > 0.6f)
			{
				Spawn_DroneSwarm(5, 0.5f);
				Spawn_Asteroid(rand() % 3, rand() % 2);
				Spawn_Asteroid(rand() % 3, rand() % 2);
				Spawn_Rusher();
				Spawn_Rusher();
			}
			else if (spawnDifficulty > 0.5f)
			{
				Spawn_Asteroid(rand() % 3, rand() % 2);
				Spawn_Asteroid(rand() % 3, rand() % 2);
				Spawn_Asteroid(rand() % 3, rand() % 2);
				Spawn_Asteroid(rand() % 3, rand() % 2);
				Spawn_Asteroid(rand() % 3, rand() % 2);
				Spawn_Asteroid(rand() % 3, rand() % 2);
				Spawn_Rusher();
				Spawn_Rusher();
			}
			else if (spawnDifficulty > 0.4f)
			{
				Spawn_Loner();
				Spawn_Asteroid(1, false);
				Spawn_Asteroid(1, false);
			}
			else if (spawnDifficulty > 0.3f)
			{
				Spawn_Loner();
				Spawn_Asteroid(2, false);
			}
			else if (spawnDifficulty > 0.2f)
			{
				Spawn_Rusher();
				Spawn_Rusher();
				Spawn_Rusher();
			}
			else if (spawnDifficulty > 0.1f)
			{
				Spawn_Rusher();
				Spawn_Rusher();
				Spawn_Asteroid(1, false);
			}
			else if (spawnDifficulty > 0.0f)
			{
				Spawn_Rusher();
				Spawn_Asteroid(2, false);
			}
			

		}

	}


};