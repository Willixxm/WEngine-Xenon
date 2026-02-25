#pragma once
#include "WEngine/Pawn.h"
#include "WEngine/GameContext.h"
#include <string>
using namespace WE;
class SpaceShip;

class PowerUp : public Pawn
{
public:
	PowerUp();

	void Start() override;
	void Update(float deltaTime) override;

	void On_SensorBeginOverlap(Entity* other) override;
	virtual bool OnPickUp(SpaceShip* ship);

protected:
	std::string filePath = "graphics/PUInvuln.bmp";
	float hitboxSizeMult = 0.8f;
	int hTiles = 4;
	int vTiles = 2;
	int tileOffset = 0;
	int tileSpan = 8;
	int renderLayer = 0;
	float animationFPS = 5.f;

	bool isSensor = true;

	std::string pickUpText = "Power UP";
	bool hasBeenPickedUp = false;

	uint32_t collisionLayer = WCollisionLayer::Layer3;
	uint32_t collidesWith = WCollisionLayer::Layer1;

	float moveSpeed = 3.f;
	float maxLifeTime = 25.0f;
};

