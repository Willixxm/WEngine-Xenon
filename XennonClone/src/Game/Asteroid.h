#pragma once
#include "Enemy.h"
class Asteroid : public Enemy
{
public:
	Asteroid();

	void Start() override;
	void Update(float deltaTime) override;
};

