#pragma once

#include "WEngine/Pawn.h"
#include "WEngine/GameContext.h"
//#include "XennonLevel.h"
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

	float lifePoints = 100.f;
	float bodyDamage = 50.f;

public:
	float moveSpeed = 2.f;
	WVec2 normalizedMoveVector = WVec2(-1, 0);
	
protected:
	float shotCooldownTime = 1.5f;
	bool canShoot = true;

	float maxLifeTime = 45.0f;

	uint32_t collisionLayer = WCollisionLayer::Layer3;
	uint32_t collidesWith = WCollisionLayer::Layer1 | WCollisionLayer::Layer2;

private:
	float timeUntilNextShot = shotCooldownTime;

};

