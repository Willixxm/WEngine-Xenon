#pragma once
#include "Enemy.h"
class AsteroidMetalBig : public Enemy
{
public:
	AsteroidMetalBig();

	void Update(float deltaTime) override;
};

