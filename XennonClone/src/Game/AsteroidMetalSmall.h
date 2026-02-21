#pragma once
#include "Enemy.h"
class AsteroidMetalSmall : public Enemy
{
public:
	AsteroidMetalSmall();

	void Update(float deltaTime) override;
};

