#pragma once
#include "Asteroid.h"
class AsteroidStoneSmall : public Asteroid
{
public:
	AsteroidStoneSmall()
	{
		filePath = "graphics/SAster32.bmp";
		hitboxSizeMult = 0.8f;
		hTiles = 8;
		vTiles = 2;
		tileOffset = 0;
		tileSpan = hTiles * vTiles;
		renderLayer = 0;
		animationFPS = 9.f;

		lifePoints = 25.f;

		moveSpeed = 0.f;

		score = 5000;
	}

	friend class AsteroidStoneMedium;
};

