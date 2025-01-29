#pragma once

#include "WEngine/Entity.h"

#include "SpaceShip.h"

class XennonLevel : public WE::Entity
{
public:
	void Start() override
	{
		WE::Entity::Start();

		//GetGameContext()->GAME_SetWorldGravity(WE::WVec2(0.f, -9.8f));

		SpaceShip* playerShip = GetGameContext()->GAME_InstantiateEntity<SpaceShip>(WE::WVec2(0, 20));

	}

	void Update(float deltaTime) override
	{
		WE::Entity::Update(deltaTime);

		


	}


};