#pragma once
#include "Asteroid.h"
class AsteroidMetalBig : public Asteroid
{
public:
	AsteroidMetalBig()
	{
		filePath = "graphics/MAster96.bmp";
		hitboxSizeMult = 0.5f;
		hTiles = 5;
		vTiles = 5;
		tileOffset = 0;
		tileSpan = hTiles * vTiles;
		renderLayer = 0;
		animationFPS = 9.f;

		initialSize = WVec2(5);

		bodyDamage = 75.f;

		lifePoints = 9999.f;
		isInvincible = true;

		isSensor = false;

		moveSpeed = 0.f;
	}
};

