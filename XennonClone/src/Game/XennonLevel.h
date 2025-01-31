#pragma once

#include "WEngine/Entity.h"
#include "WEngine/GameContext.h"

#include "SpaceShip.h"
#include "BackGround.h"
#include "Enemy.h"

class XennonLevel : public WE::Entity
{
public:
	void Start() override
	{
		WE::Entity::Start();

		//GetGameContext()->GAME_SetWorldGravity(WE::WVec2(0.f, -9.8f));
		GetGameContext()->RENDER_SetOrthoCameraSize(35.f);

		SpaceShip* playerShip = GetGameContext()->GAME_InstantiateEntity<SpaceShip>(WE::WVec2(0, -10), WE::WVec2(5));

		BackGround* background1 = GetGameContext()->GAME_InstantiateEntity<BackGround>(WE::WVec2(0, 0), WE::WVec2(0));
		
		Enemy* enemy = GetGameContext()->GAME_InstantiateEntity<Enemy>(WE::WVec2(0, 10), WE::WVec2(6));
	}

	void Update(float deltaTime) override
	{
		WE::Entity::Update(deltaTime);

		


	}


};