#pragma once
#include "Asteroid.h"
class AsteroidMetalSmall : public Asteroid
{
public:
	AsteroidMetalSmall()
	{
		filePath = "graphics/MAster32.bmp";
		hitboxSizeMult = 0.65f;
		hTiles = 8;
		vTiles = 2;
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

