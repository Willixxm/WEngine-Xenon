#pragma once

#include "WEngine/Entity.h"
#include "WEngine/WMath.h"
#include <string>
#include <vector>

using namespace WE;

class ParallaxBackground : public Entity
{
public:

	void Start() override;
	void Update(float deltaTime) override;

	void CreateTiles();

protected:
	std::string filePath = "graphics/Blocks.bmp";

	float tileSizeMult = 5.f;
	int layer = -20;

	float scrollVel = 4.f;

	std::vector<Entity*> tileEntities;


};
