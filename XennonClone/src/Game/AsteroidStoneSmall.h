#pragma once
#include "Enemy.h"
class AsteroidStoneSmall : public Enemy
{
public:
	AsteroidStoneSmall();

	void Update(float deltaTime) override;
};

