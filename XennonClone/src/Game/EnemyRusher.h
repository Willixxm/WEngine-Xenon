#pragma once

#include "Enemy.h"

class EnemyRusher : public Enemy
{
public:
	EnemyRusher();

	void Update(float deltaTime) override;
	void DieByPlayer() override;
};

