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

private:
	void CreateTiles();
	void GenTileFromArray(const std::vector<double>& tileIDs, int x_offset, int y_offset, int layer, std::vector<WE::Entity*>& entities);

protected:
	std::string filePath = "graphics/Blocks.bmp";

	int layer_close = -20;
	float scrollVel_close = 4.f;
	std::vector<Entity*> tileEntities_close;

	int layer_far = -40;
	float scrollVel_far = 1.5f;
	std::vector<Entity*> tileEntities_far;

};
