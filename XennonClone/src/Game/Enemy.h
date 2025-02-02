#pragma once

#include "WEngine/Pawn.h"
#include <string>

using namespace WE;

class Enemy : public Pawn
{

public:

	void Start() override;
	void Update(float deltaTime) override;

	void DealDamage(float damage) override;

protected:
	std::string filePath = "graphics/explode64.bmp";
	float hitboxSizeMult = 1.0f;
	int hTiles = 5;
	int vTiles = 2;
	float lifePoints = 100.f;

};

