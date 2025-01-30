#pragma once

#include "WEngine/Entity.h"
#include <string>



class BackGround : public WE::Entity
{
public:

	void Start() override;
	void Update(float deltaTime) override;

protected:
	std::string filePath = "graphics/galaxy2.bmp";
	int layer = -20;
	float scrollVel = -5;


};