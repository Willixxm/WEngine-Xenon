#include "SpaceShip.h"
#include "WEngine/GameContext.h"
#include "WEngine/InputVectorComponent.h"
#include "WEngine/WMath.h"
#include "WEngine/Pawn.h"
#include <iostream>

#include "PlayerProjectile.h"

using namespace WE;

void SpaceShip::Start() 
{
	Pawn::Start();

	GetGameContext()->PHYS_AddPhysComponentToEntity(this, WE::WBodyType::dynamicBody, GetInitialSize()*0.75f);
	GetGameContext()->RENDER_AddRenderComponent(this, "graphics/Ship2.bmp", 7, 3, 0, 7, 0);
	GetGameContext()->RENDER_SetAnimationParameters(this, false, 2.f);

}

void SpaceShip::Update(float deltaTime) 
{
	Pawn::Update(deltaTime);

	
	HandleMovement(deltaTime);
	HandleShoot(deltaTime);

}



void SpaceShip::HandleShoot(float deltaTime)
{
	if (timeUntilNextShot <= 0)
	{
		if (GetGameContext()->INPUT_GetJoyButtonDown(INPUT_JoyButtonCode::FaceDown)
			|| GetGameContext()->INPUT_GetKeyDown(INPUT_KeyCode::MouseLeft)
			|| GetGameContext()->INPUT_GetKeyDown(INPUT_KeyCode::Spacebar) )
		{
			PrimaryFire();
			timeUntilNextShot = fireCoolDown;
		}
		else if (GetGameContext()->INPUT_GetJoyButtonDown(INPUT_JoyButtonCode::FaceLeft)
			|| GetGameContext()->INPUT_GetKeyDown(INPUT_KeyCode::MouseRight))
		{
			//TODO: secondary fire
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
		GetGameContext()->INPUT_GetKeyDown(WE::INPUT_KeyCode::A) * -1 + GetGameContext()->INPUT_GetKeyDown(WE::INPUT_KeyCode::D),
		GetGameContext()->INPUT_GetKeyDown(WE::INPUT_KeyCode::S) * -1 + GetGameContext()->INPUT_GetKeyDown(WE::INPUT_KeyCode::W));

	inputVectorComponent.AddInputValue(WE::InputType::Axis,
		GetGameContext()->INPUT_GetJoyAxisValue(WE::INPUT_JoyAxisCode::LeftStick_Horizontal),
		GetGameContext()->INPUT_GetJoyAxisValue(WE::INPUT_JoyAxisCode::LeftStick_Vertical) * -1);

	WE::WVec2 inputVec = inputVectorComponent.GetInputVectorAndReset();

	Move(inputVec * moveSpeed);

	GetGameContext()->RENDER_SetManualAnimationState(this, inputVec.x / 2 + 0.5f);
	
}

void SpaceShip::PrimaryFire()
{
	GetGameContext()->GAME_InstantiateEntity<PlayerProjectile>(GetLocation() + WE::WVec2(0, 2), WE::WVec2(2));



}