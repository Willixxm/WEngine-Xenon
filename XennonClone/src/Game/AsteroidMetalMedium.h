#pragma once
#include "Asteroid.h"
class AsteroidMetalMedium : public Asteroid
{
public:
	AsteroidMetalMedium()
	{
		filePath = "graphics/MAster64.bmp";
		hitboxSizeMult = 0.65f;
		hTiles = 8;
		vTiles = 3;
		tileOffset = 0;
		tileSpan = hTiles * vTiles;
		renderLayer = 0;
		animationFPS = 9.f;

		lifePoints = 9999.f;
		isInvincible = true;

		isSensor = false;

		moveSpeed = 0.f;
	}
};

