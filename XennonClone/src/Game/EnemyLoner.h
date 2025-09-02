#pragma once

#include "Enemy.h"


class EnemyLoner : public Enemy
{
public:

	EnemyLoner();

	void Start() override;
	void Update(float deltaTime) override;

	void Fire() override;

private:
	float projectileSpeed = 10.f;
};

