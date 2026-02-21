#pragma once

#include "Enemy.h"

class EnemyDrone : public Enemy
{
public:
	EnemyDrone();

	void Update(float deltaTime) override;
};

