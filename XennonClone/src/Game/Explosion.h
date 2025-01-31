#pragma once

#include "WEngine/Entity.h"

#include <string>

class Explosion : public WE::Entity
{
public:
	void Start() override;

	void Update(float deltaTime) override;

protected:
	std::string filePath = "graphics/explode64.bmp";
	int hTiles = 5;
	int vTiles = 2;
	float lifeTimeMult = 0.1f;

private:
	float lifeTime = 1;

};

