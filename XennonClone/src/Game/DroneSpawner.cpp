#include "DroneSpawner.h"
#include "WEngine/GameContext.h"
#include "EnemyDrone.h"
#include "EnemyDrone.h"



void DroneSpawner::StartSpawning()
{
	GetGameContext()->GAME_StartCoroutine(this, CoroutineID::DroneSpawning, enemiesToSpawn * spawnInterval);
}

void DroneSpawner::OnCoroutineUpdate(int ID, float duration)
{
	switch (ID)
	{
	case CoroutineID::DroneSpawning:
		if (duration > spawnedEnemies * spawnInterval)
		{
			EnemyDrone* enemy = GetGameContext()->GAME_InstantiateEntity<EnemyDrone>(GetLocation(), 0.f, GetInitialSize());
			enemy->SetLevelInstance(levelInstance);
			enemy->moveVector = moveVector;
			
			if (lastSpawnedEnemy)
			{
				enemy->previousDroneInGroup = lastSpawnedEnemy;
				lastSpawnedEnemy->nextDroneInGroup = enemy;
				enemy->deathCountInGroup = lastSpawnedEnemy->deathCountInGroup;
			}

			lastSpawnedEnemy = enemy;

			++spawnedEnemies;
		}
		break;
	}
}

void DroneSpawner::OnCoroutineEnd(int ID)
{
	switch (ID)
	{
	case CoroutineID::DroneSpawning:
		Destroy();
		break;
	}
}

