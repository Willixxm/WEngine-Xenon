#pragma once

#include "Enemy.h"


class EnemyProjectile : public Enemy
{
public:

	EnemyProjectile();

	void Update(float deltaTime) override;

	virtual void DieByPlayer() override;

};

