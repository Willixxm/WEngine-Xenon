#pragma once
#include "Enemy.h"
class AsteroidMetalMedium : public Enemy
{
public:
	AsteroidMetalMedium();

	void Update(float deltaTime) override;
};

