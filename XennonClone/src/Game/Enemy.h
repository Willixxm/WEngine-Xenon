#pragma once

#include "WEngine/Pawn.h"
#include "WEngine/GameContext.h"
#include <string>

using namespace WE;

class XennonLevel;

class Enemy : public Pawn
{

public:

	void Start() override;
	void Update(float deltaTime) override;

	void DealDamage(Entity* dealer, float damage) override;

	void On_SensorBeginOverlap(Entity* other) override;

	void SetLevelInstance(XennonLevel* instance) { levelInstance = instance; }
	XennonLevel* GetLevelInstance() const { return levelInstance; }

	float GetAnimFps() const { return animationFPS; }

private:
	XennonLevel* levelInstance = nullptr;

protected:
	virtual void DieByPlayer();

	virtual void HandleShoot(float deltaTime);
	virtual void Fire();

	void HandleEnemyLifeTime();


protected:
	std::string filePath = "graphics/font16x16.bmp";
	float hitboxSizeMult = 0.8f;
	int hTiles = 8;
	int vTiles = 12;
	int tileOffset = 46;
	int tileSpan = 2;
	int renderLayer = 0;
	float animationFPS = 2.5f;

	bool isSensor = true;

	bool isInvincible = false;
	float lifePoints = 100.f;
	float bodyDamage = 50.f;

public:
	float moveSpeed = 2.f;
	WVec2 normalizedMoveVector = WVec2(-1, 0);

	float shotCooldownTime = 1.5f;
	bool canShoot = false;

	float maxLifeTime = 40.0f;

protected:
	uint32_t collisionLayer = WCollisionLayer::Layer3;
	uint32_t collidesWith = WCollisionLayer::Layer1 | WCollisionLayer::Layer2;

	float timeUntilNextShot = shotCooldownTime;

};

