#pragma once

#include "WEngine/Entity.h"
#include <string>

using namespace WE;

struct LifeIcon
{
	Entity* icon;
	uint32_t id;
};

class LifeCountDisplay : public Entity
{
public:
	void SetExtraLifeCount(int extraLives);

protected:
	void UpdateIcons();

	std::string filePath = "graphics/PULife.bmp";
	int hTiles = 1;
	int vTiles = 1;
	int tileOffset = 0;
	int tileSpan = 1;

	float iconOffsetMult = 0.8f;
	float iconRotation = 0.f;

private:
	int extraLifeCount = 0;
	std::vector<LifeIcon> icons;

};

