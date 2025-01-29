#pragma once

#include "WEngine/Entity.h"


class SpaceShip : public WE::Entity
{
public:
	
	void Start() override;
	void Update(float deltaTime) override;

private:
	float timeUntilNextShot = 0.f;
	const float fireCoolDown = 0.5f;



private:
	void HandleShoot(float deltaTime);

};

