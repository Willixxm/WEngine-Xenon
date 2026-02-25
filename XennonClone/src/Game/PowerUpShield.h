#pragma once
#include "PowerUp.h"
#include "SpaceShip.h"
class PowerUpShield : public PowerUp
{
public:
	PowerUpShield()
	{
		filePath = "graphics/PUShield.bmp";

		pickUpText = "+Health";
	}

	bool OnPickUp(SpaceShip* ship) override
	{
		ship->OnPickUp_Shield();
		return true;
	}
};

