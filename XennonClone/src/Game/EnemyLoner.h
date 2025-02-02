#pragma once

#include "Enemy.h"


class EnemyLoner : public Enemy
{
public:

	EnemyLoner();

	//void Start() override;
	void Update(float deltaTime) override;

	//void DealDamage(float damage) override;

private:
	float ySpeed = -1.f;

};

