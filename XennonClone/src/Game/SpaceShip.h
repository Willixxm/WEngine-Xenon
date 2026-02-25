#pragma once

#include "WEngine/Pawn.h"
#include "WEngine/InputVectorComponent.h"
#include <string>

class TextRenderer;
class Companion;
class PowerUp;
class WE::WVec2;

struct CompanionRef
{
	Companion* comp = nullptr;
	uint32_t id = 0;
};

class SpaceShip : public WE::Pawn
{
public:
	
	void Start() override;
	void Update(float deltaTime) override;

	void DealDamage(Entity* dealer, float damage) override;

	void OnCoroutineUpdate(int ID, float duration) override;
	void OnCoroutineEnd(int ID) override;

	float GetCurrentHealth() const { return currentHealth; }
	float GetMaxHealth() const { return maxHealth; }

	void OnPickUp_Shield();
	void OnPickUp_Weapon();
	virtual bool OnPickUp_Companion(const PowerUp* powerUp);
	void OnRemoveCompanion(const Companion* companionToRemove);

	bool HasThisCompanion(const Companion* comp, uint32_t compID, int slot);

	void Destroy() override;
	


protected:
	std::string filePath = "graphics/Ship2.bmp";

	int hTiles = 7;
	int vTiles = 3;
	int tileOffset = 0;
	int tileSpan = 7;
	int layer = 0;

	int flashTileOffset = 14;
	int flashTileSpan = 7;

	bool canShoot = true;
	float timeUntilNextShot = 0.f;
	const float fireCoolDown = 0.25f;
	float fireCoolDownMult = 1.f;
	unsigned int weaponLevel = 0;

	float moveSpeed = 20.f;

	float maxHealth = 200.f;
	float currentHealth = maxHealth;
	float dmgFlashDuration = 0.1f;

	bool isInvincible = false;
	float invincibilityDuration = 3.0f;
	bool canRespawn = true;

	TextRenderer* inputVectorText = nullptr;

	Entity* healthBar = nullptr;
	uint32_t healthBarID = 0;

private:
	void HandleShoot(float deltaTime);
	WE::InputVectorComponent inputVectorComponent;
	virtual void HandleMovement(float deltaTime);

	void PrimaryFire();


	CompanionRef companions[2]{};

	

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

