#pragma once

#include "WEngine/Pawn.h"
#include "WEngine/InputVectorComponent.h"

class TextRenderer;

class SpaceShip : public WE::Pawn
{
public:
	
	void Start() override;
	void Update(float deltaTime) override;

	void DealDamage(Entity* dealer, float damage) override;

	void OnCoroutineUpdate(int ID, float duration) override;
	void OnCoroutineEnd(int ID) override;

private:
	float timeUntilNextShot = 0.f;
	const float fireCoolDown = 0.15f;
	float moveSpeed = 17.5f;

	bool isInvincible = false;
	float invincibilityDuration = 0.76f;

	TextRenderer* inputVectorText = nullptr;

private:
	void HandleShoot(float deltaTime);
	WE::InputVectorComponent inputVectorComponent;
	void HandleMovement(float deltaTime);

	void PrimaryFire();

private:
	enum CoroutineID : int
	{
		invincibleAfterDmg = 0
	};

private:
	bool textRendererTest = false;

};

