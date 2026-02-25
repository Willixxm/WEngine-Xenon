#pragma once
#include "WEngine/Entity.h"

using namespace WE;

class XennonLevel;
class EnemyDrone;

class DroneSpawner : public Entity
{
public:
	void StartSpawning();

	void OnCoroutineUpdate(int ID, float duration) override;
	void OnCoroutineEnd(int ID) override;

	void SetLevelInstance(XennonLevel* instance) { levelInstance = instance; }
	XennonLevel* GetLevelInstance() const { return levelInstance; }


	WVec2 moveVector = WVec2(-1, 0);
	int enemiesToSpawn = 5;
	float spawnInterval = 0.5f;

private:
	XennonLevel* levelInstance = nullptr;

	int spawnedEnemies = 0;
	EnemyDrone* lastSpawnedEnemy = nullptr;
	uint32_t lastSpawnedEnemyID = 0;

private:
	enum CoroutineID : int
	{
		DroneSpawning = 0
	};

};

