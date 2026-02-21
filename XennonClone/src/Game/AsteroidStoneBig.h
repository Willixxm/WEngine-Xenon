#pragma once

#include "Enemy.h"

class AsteroidStoneBig : public Enemy
{
public:
	AsteroidStoneBig();

	void Update(float deltaTime) override;
};

