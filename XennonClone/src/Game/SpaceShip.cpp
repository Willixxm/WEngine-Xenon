#include "SpaceShip.h"
#include "WEngine/GameContext.h"
#include "WEngine/InputVectorComponent.h"
#include "WEngine/WMath.h"
#include <iostream>

void SpaceShip::Start() 
{
	WE::Entity::Start();

	GetGameContext()->PHYS_AddPhysComponentToEntity(this, WE::WBodyType::dynamicBody, WE::WVec2(1, 1));
	GetGameContext()->RENDER_AddRenderComponent(this, "graphics/Ship1.bmp", 7, 1, 5, 5, WE::WRenderType::Render_Surface, 0);
	GetGameContext()->RENDER_SetAnimationParameters(this, false, 2.f);

}

void SpaceShip::Update(float deltaTime) 
{
	WE::Entity::Update(deltaTime);

	
	HandleMovement(deltaTime);

	HandleShoot(deltaTime);

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

void SpaceShip::HandleMovement(float deltaTime)
{
	inputVectorComponent.AddInputValue(WE::InputType::Key,
		GetGameContext()->INPUT_GetKeyDown(WE::INPUT_KeyCode::Key_A) * -1 + GetGameContext()->INPUT_GetKeyDown(WE::INPUT_KeyCode::Key_D),
		GetGameContext()->INPUT_GetKeyDown(WE::INPUT_KeyCode::Key_S) * -1 + GetGameContext()->INPUT_GetKeyDown(WE::INPUT_KeyCode::Key_W));

	inputVectorComponent.AddInputValue(WE::InputType::Axis,
		GetGameContext()->INPUT_GetJoyAxisValue(WE::INPUT_JoyAxisCode::LeftStick_Horizontal),
		GetGameContext()->INPUT_GetJoyAxisValue(WE::INPUT_JoyAxisCode::LeftStick_Vertical) * -1);

	WE::WVec2 inputVec = inputVectorComponent.GetInputVectorAndReset();
	GetGameContext()->PHYS_SetLinearVelocityOnPhysObj(this->bodyId, inputVec * 10);
	GetGameContext()->RENDER_SetManualAnimationState(this, inputVec.x / 2 + 0.5f);
	
	
}