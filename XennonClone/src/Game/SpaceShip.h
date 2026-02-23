#pragma once

#include "WEngine/Pawn.h"
#include "WEngine/InputVectorComponent.h"

class TextRenderer;
class WE::WVec2;

class SpaceShip : public WE::Pawn
{
public:
	
	void Start() override;
	void Update(float deltaTime) override;

	void DealDamage(Entity* dealer, float damage) override;

	//void On_SensorBeginOverlap(Entity* other) override;
	//void On_CollisionBegin(Entity* other, WE::WVec2 point) override;

	void OnCoroutineUpdate(int ID, float duration) override;
	void OnCoroutineEnd(int ID) override;

	float GetCurrentHealth() const { return currentHealth; }
	float GetMaxHealth() const { return maxHealth; }

private:
	float timeUntilNextShot = 0.f;
	const float fireCoolDown = 0.15f;
	float moveSpeed = 17.5f;

	float maxHealth = 500.f;
	float currentHealth = maxHealth;
	float dmgFlashDuration = 0.1f;

	bool isInvincible = false;
	float invincibilityDuration = 3.0f;

	TextRenderer* inputVectorText = nullptr;

	Entity* healthBar = nullptr;

private:
	void HandleShoot(float deltaTime);
	WE::InputVectorComponent inputVectorComponent;
	void HandleMovement(float deltaTime);

	void PrimaryFire();

private:
	enum CoroutineID : int
	{
		invincibleAfterDeath = 0,
		flashAfterDamage = 1
	};

private:
	bool textRendererTest = false;

	friend class XennonLevel;
};

