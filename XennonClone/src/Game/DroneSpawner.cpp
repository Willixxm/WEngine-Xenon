#include "DroneSpawner.h"
#include "WEngine/GameContext.h"
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
			EnemyDrone* enemy = GetGameContext()->GAME_InstantiateEntity<EnemyDrone>(GetLocation(), 0.f);
			enemy->SetLevelInstance(levelInstance);
			enemy->moveVector = moveVector;
			
			if (GetGameContext()->IsValid(lastSpawnedEnemy, lastSpawnedEnemyID))
			{
				enemy->previousDroneInGroup = lastSpawnedEnemy;
				enemy->previousDroneInGroupID = lastSpawnedEnemyID;

				lastSpawnedEnemy->nextDroneInGroup = enemy;
				lastSpawnedEnemy->nextDroneInGroupID = enemy->GetID();

				enemy->deathCountInGroup = lastSpawnedEnemy->deathCountInGroup;
			}

			lastSpawnedEnemy = enemy;
			lastSpawnedEnemyID = enemy->GetID();

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

