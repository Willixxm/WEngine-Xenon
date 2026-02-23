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
	EnemyDrone* previousDroneInGroup = nullptr;
	
	int initScore;

	friend class DroneSpawner;
	friend class EnemyDrone;

};

