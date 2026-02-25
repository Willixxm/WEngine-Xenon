#pragma once
#include "PowerUp.h"
#include "SpaceShip.h"
#include "Companion.h"
class PowerUpCompanion : public PowerUp
{
public:
	PowerUpCompanion()
	{
		filePath = "graphics/clone.bmp";

		hTiles = 4;
		vTiles = 5;
		tileOffset = 0;
		tileSpan = 16;
		//initialSize = 4.f;

		pickUpText = "+Companion";
	}

	bool OnPickUp(SpaceShip* ship) override
	{
		return ship->OnPickUp_Companion(this);
	}
};

