#include "SpaceShip.h"
#include "WEngine/GameContext.h"
#include <iostream>

void SpaceShip::Start() 
{
	WE::Entity::Start();

	GetGameContext()->GAME_AddPhysComponentToEntity(this, WE::WBodyType::dynamicBody, WE::WVec2(1, 1));

}

void SpaceShip::Update(float deltaTime) 
{
	WE::Entity::Update(deltaTime);

	HandleShoot(deltaTime);

	std::cout << GetLocation().x << " / " << GetLocation().y << '\n';
}



void SpaceShip::HandleShoot(float deltaTime)
{
	if (timeUntilNextShot <= 0)
	{
		if (GetGameContext()->INPUT_GetJoyButtonDown(WE::INPUT_JoyButtonCode::ButtonFaceDown) || GetGameContext()->INPUT_GetKeyDown(WE::INPUT_KeyCode::Key_MouseLeft))
		{
			std::cout << "FIRE \n";
			timeUntilNextShot = fireCoolDown;
		}
	}
	else
	{
		timeUntilNextShot -= deltaTime;
	}
}