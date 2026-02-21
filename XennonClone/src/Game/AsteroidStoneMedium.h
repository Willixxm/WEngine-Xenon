#pragma once
#include "Enemy.h"
class AsteroidStoneMedium : public Enemy
{
public:
	AsteroidStoneMedium();

	void Update(float deltaTime) override;
};

