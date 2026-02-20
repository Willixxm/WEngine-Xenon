#include "SpaceShip.h"
#include "WEngine/GameContext.h"
#include "WEngine/InputVectorComponent.h"
#include "WEngine/WMath.h"
#include "WEngine/Pawn.h"
#include <iostream>
#include <cmath>

#include "PlayerProjectile.h"
#include "Explosion.h"
#include "TextRenderer.h"

using namespace WE;

void SpaceShip::Start() 
{
	Pawn::Start();

	GetGameContext()->PHYS_AddPhysComponentToEntity(this, WE::WBodyType::dynamicBody, GetInitialSize()*0.75f, WCollisionLayer::Layer1, WCollisionLayer::Layer3, false, 1.f);
	GetGameContext()->RENDER_AddRenderComponent(this, "graphics/Ship2.bmp", 7, 3, 0, 7, 0);
	GetGameContext()->RENDER_SetAnimationParameters(this, false, 2.f);

	if (textRendererTest)
		inputVectorText = GetGameContext()->GAME_InstantiateEntity<TextRenderer>(GetLocation() + WVec2(0, 2.f), 0.f, WVec2(0.5f));
}

void SpaceShip::Update(float deltaTime) 
{
	Pawn::Update(deltaTime);

	HandleMovement(deltaTime);
	HandleShoot(deltaTime);

	//std::cout << GetLocation().x << " | " << GetLocation().y << '\n';
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

	GetGameContext()->RENDER_SetManualAnimationState(this, -inputVec.y / 2 + 0.5f);

	if (inputVectorText)
	{
		std::string inputBar = "O";
		for (int i = 0; i < inputVec.length() * 5; ++i)
			inputBar += ":";
		inputBar += ">";
		inputVectorText->SetLocation(GetLocation());
		inputVectorText->SetRotation(atan2(inputVec.y, inputVec.x));
		inputVectorText->SetText(inputBar);
	}
}

void SpaceShip::PrimaryFire()
{
	auto muzzleLocation = GetLocation() + GetUpVector() * 1.5f;
	auto projectile = GetGameContext()->GAME_InstantiateEntity<PlayerProjectile>(muzzleLocation, GetRotation(), WE::WVec2(2));

	if (textRendererTest)
	{
		auto bangText = GetGameContext()->GAME_InstantiateEntity<TextRenderer>(muzzleLocation + WVec2(0, 1), 0, WVec2(0.5));
		bangText->SetText("Pew!");
		bangText->DestroyWithFloat(WVec2(0, 2), 0.5);
	}
	

}

void SpaceShip::DealDamage(Entity* dealer, float damage)
{
	if (!isInvincible)
	{
		GetGameContext()->GAME_StartCoroutine(this, CoroutineID::invincibleAfterDmg, invincibilityDuration);
		GetGameContext()->RENDER_SetAnimationTileParameters(this, 7, 7);
		isInvincible = true;

		GetGameContext()->GAME_InstantiateEntity<Explosion>(GetLocation(), 0.f, GetInitialSize());
	}
	
}


void SpaceShip::OnCoroutineUpdate(int ID, float duration)
{
	switch (ID)
	{
	case CoroutineID::invincibleAfterDmg:
		if ((int)(duration*10) % 2 == 0)
			GetGameContext()->RENDER_SetAnimationTileParameters(this, 0, 7);
		else
			GetGameContext()->RENDER_SetAnimationTileParameters(this, 7, 7);
		break;

	}

}

void SpaceShip::OnCoroutineEnd(int ID)
{
	switch (ID)
	{
	case CoroutineID::invincibleAfterDmg:
		GetGameContext()->RENDER_SetAnimationTileParameters(this, 0, 7);
		isInvincible = false;
		break;

	}

}