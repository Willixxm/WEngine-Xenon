#pragma once
#include "PowerUp.h"
#include "SpaceShip.h"
class PowerUpWeapon : public PowerUp
{
public:
	PowerUpWeapon()
	{
		filePath = "graphics/PUWeapon.bmp";

		pickUpText = "+Weapon";
	}

	bool OnPickUp(SpaceShip* ship) override
	{
		ship->OnPickUp_Weapon();
		return true;
	}
};

