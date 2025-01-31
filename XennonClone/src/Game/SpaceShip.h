#pragma once

#include "WEngine/Pawn.h"
#include "WEngine/InputVectorComponent.h"


class SpaceShip : public WE::Pawn
{
public:
	
	void Start() override;
	void Update(float deltaTime) override;

private:
	float timeUntilNextShot = 0.f;
	const float fireCoolDown = 0.2f;
	float moveSpeed = 10.f;

private:
	void HandleShoot(float deltaTime);
	WE::InputVectorComponent inputVectorComponent;
	void HandleMovement(float deltaTime);


	void PrimaryFire();
};

