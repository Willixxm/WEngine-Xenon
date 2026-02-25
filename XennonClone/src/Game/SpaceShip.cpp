#include "SpaceShip.h"
#include "WEngine/GameContext.h"
#include "WEngine/InputVectorComponent.h"
#include "WEngine/WMath.h"
#include "WEngine/Pawn.h"
#include <iostream>
#include <cmath>

#include "PlayerProjectile.h"
#include "PlayerProjectile2.h"
#include "PlayerProjectile3.h"

#include "Explosion.h"
#include "TextRenderer.h"
#include "PowerUp.h"
#include "Companion.h"

#include "Enemy.h"

using namespace WE;

void SpaceShip::Start() 
{
	Pawn::Start();

	GetGameContext()->PHYS_AddPhysComponentToEntity(this, WE::WBodyType::dynamicBody, GetInitialSize()*0.75f, WCollisionLayer::Layer1, WCollisionLayer::Layer3, false, 0.2f);
	GetGameContext()->RENDER_AddRenderComponent(this, filePath, hTiles, vTiles, tileOffset, tileSpan, layer);
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
	if (timeUntilNextShot <= 0 )
	{
		if (!canShoot)
			return;

		if (GetGameContext()->INPUT_GetJoyButtonDown(INPUT_JoyButtonCode::FaceDown)
			|| GetGameContext()->INPUT_GetKeyDown(INPUT_KeyCode::MouseLeft)
			|| GetGameContext()->INPUT_GetKeyDown(INPUT_KeyCode::Spacebar) )
		{
			PrimaryFire();
			timeUntilNextShot = fireCoolDown * fireCoolDownMult;
		}
		else if (GetGameContext()->INPUT_GetJoyButtonDown(INPUT_JoyButtonCode::FaceLeft)
			|| GetGameContext()->INPUT_GetKeyDown(INPUT_KeyCode::MouseRight))
		{
			//Secondary fire
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

	if (GetLocation().x < -38)
		SetLocation(WVec2(-38, GetLocation().y));

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
	switch (weaponLevel)
	{
	default: 
	case 2: { // base projectile
		auto muzzleLocation = GetLocation() + GetUpVector() * 1.5f;
		auto projectile = GetGameContext()->GAME_InstantiateEntity<PlayerProjectile3>(muzzleLocation, GetRotation());
	}	break;
	case 1: { // base projectile
		auto muzzleLocation = GetLocation() + GetUpVector() * 1.5f;
		auto projectile = GetGameContext()->GAME_InstantiateEntity<PlayerProjectile2>(muzzleLocation, GetRotation());
	}	break;
	case 0: { // base projectile
		auto muzzleLocation = GetLocation() + GetUpVector() * 1.5f;
		auto projectile = GetGameContext()->GAME_InstantiateEntity<PlayerProjectile>(muzzleLocation, GetRotation());
	}	break;
	}
}

void SpaceShip::DealDamage(Entity* dealer, float damage)
{
	if (!isInvincible)
	{
		currentHealth -= damage;
		if (currentHealth <= 0)
		{ // DIE
			for (size_t i = 0; i < sizeof(companions) / sizeof(companions[0]); ++i)
			{
				companions[i].comp = nullptr;
				companions[i].id = 0;
			}
			
			if (canRespawn)
			{
				GetGameContext()->GAME_StartCoroutine(this, CoroutineID::invincibleAfterDeath, invincibilityDuration);
				GetGameContext()->RENDER_SetAnimationTileParameters(this, 7, 7);
				GetGameContext()->GAME_InstantiateEntity<Explosion>(GetLocation(), 0.f, GetInitialSize() * 2.f);
				isInvincible = true;
				canShoot = false;
			}
			else
			{
				Destroy();
			}
		}
		else
		{ // TAKE DAMAGE
			GetGameContext()->GAME_StartCoroutine(this, CoroutineID::flashAfterDamage, dmgFlashDuration);
			GetGameContext()->RENDER_SetAnimationTileParameters(this, flashTileOffset, flashTileSpan);
			isInvincible = true; // very short invincibility
		}

		if (GetGameContext()->IsValid(healthBar, healthBarID))
			GetGameContext()->RENDER_SetManualAnimationState(healthBar, (maxHealth - currentHealth) / maxHealth);

	}
}


void SpaceShip::OnCoroutineUpdate(int ID, float duration)
{
	switch (ID)
	{
	case CoroutineID::invincibleAfterDeath:
	{
		float coroutineProgress = duration / invincibilityDuration;
		if (coroutineProgress < 0.5f)
		{
			SetLocation(WVec2(-50, 0) + WVec2(30, 0) * coroutineProgress * 2);
			GetGameContext()->RENDER_SetManualAnimationState(healthBar, 1 - coroutineProgress * 2);
		}
		else if (!canShoot)
		{
			GetGameContext()->RENDER_SetManualAnimationState(healthBar, 0);
			canShoot = true;
		}
		
		if ((int)(duration * 10) % 2 == 0)
			GetGameContext()->RENDER_SetAnimationTileParameters(this, 0, 7);
		else
			GetGameContext()->RENDER_SetAnimationTileParameters(this, 7, 7);
	}
	break;
	}
}

void SpaceShip::OnCoroutineEnd(int ID)
{
	switch (ID)
	{
	case CoroutineID::invincibleAfterDeath:
		GetGameContext()->RENDER_SetAnimationTileParameters(this, tileOffset, tileSpan);
		currentHealth = maxHealth;
		GetGameContext()->RENDER_SetManualAnimationState(healthBar, (maxHealth - currentHealth) / maxHealth);
		isInvincible = false;
		break;
	case CoroutineID::flashAfterDamage:
		GetGameContext()->RENDER_SetAnimationTileParameters(this, tileOffset, tileSpan);
		isInvincible = false;
		break;
	
	}
}

void SpaceShip::OnPickUp_Shield()
{
	currentHealth = maxHealth;
	if (GetGameContext()->IsValid(healthBar, healthBarID))
		GetGameContext()->RENDER_SetManualAnimationState(healthBar, (maxHealth - currentHealth) / maxHealth);
}
void SpaceShip::OnPickUp_Weapon()
{
	weaponLevel++;

	switch (weaponLevel)
	{
	default: { // base projectile
		fireCoolDownMult = (2.5f + 2) / (weaponLevel + 2);
	}	break;
	case 2: 
	case 1: 
		fireCoolDownMult = 0.8f;
		break;
	case 0: 
		fireCoolDownMult = 1.f;
		break;
	}
}

bool SpaceShip::OnPickUp_Companion(const PowerUp* powerUp)
{
	//std::cout << "PICK UP COMPANION ==\n";

	for (size_t i = 0; i < (sizeof(companions) / sizeof(companions[0])); ++i)
	{
		//std::cout << "checking slot:" << i << " -> ";
		if (GetGameContext()->IsValid(companions[i].comp, companions[i].id))
		{
			//std::cout << "valid companion [" << companions[i].comp << "] with slot id:" << companions[i].comp->GetSlot() << '\n';
			if (companions[i].comp->GetSlot() == i)
				continue;
				
			//std::cout << "COMPANION IS FILLING TWO SLOTS! \n";
		}
		
		// if empty slot OR filled by companion with different slot ID (DANGLING POINTER)
		
		//std::cout << "empty slot" << '\n';
			
		auto companion = GetGameContext()->GAME_InstantiateEntity<Companion>(powerUp->GetLocation(), GetRotation(), powerUp->GetInitialSize());
		//std::cout << "Instantiating new Companion [" << companion << "] // in slot: " << i << '\n';
		companions[i].comp = companion;
		companions[i].id = companion->GetID();
		companion->SetPlayer(this, GetID(), i);
		return true;
	}

	//std::cout << "returning NOT VALID on_pickup..." << '\n';
	return false;
}

void SpaceShip::OnRemoveCompanion(const Companion* companionToRemove)
{
	for (size_t i = 0; i < (sizeof(companions) / sizeof(companions[0])); ++i)
	{
		if (companions[i].comp == companionToRemove)
		{
			companions[i].comp = nullptr;
			companions[i].id = 0;
		}

	}
}

bool SpaceShip::HasThisCompanion(const Companion* comp, uint32_t compID, int slot)
{
	return (companions[slot].comp == comp && companions[slot].id == compID);
}

void SpaceShip::Destroy()
{
	GetGameContext()->GAME_InstantiateEntity<Explosion>(GetLocation(), 0.f, GetInitialSize() * 2.f);
	if (GetGameContext()->IsValid(healthBar, healthBarID))
		healthBar->Destroy();
	
	Entity::Destroy();
}