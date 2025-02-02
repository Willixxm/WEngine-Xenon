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

	virtual void On_Die();

protected:
	std::string filePath = "graphics/font16x16.bmp";
	float hitboxSizeMult = 1.0f;
	int hTiles = 8;
	int vTiles = 12;
	int tileOffset = 46;
	int tileSpan = 2;
	int renderLayer = 0;
	float animationFPS = 2.5f;

	float lifePoints = 100.f;


};

