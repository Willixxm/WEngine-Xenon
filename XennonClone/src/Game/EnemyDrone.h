#pragma once

#include "Enemy.h"

class EnemyDrone : public Enemy
{
public:
	EnemyDrone();

	void Destroy() override;
	void DieByPlayer() override;

	void Update(float deltaTime) override;

private:
	int deathCountInGroup = 0;

	EnemyDrone* nextDroneInGroup = nullptr;
	uint32_t nextDroneInGroupID = 0;
	EnemyDrone* previousDroneInGroup = nullptr;
	uint32_t previousDroneInGroupID = 0;
	
	int initScore;

	friend class DroneSpawner;
	friend class EnemyDrone;
};

