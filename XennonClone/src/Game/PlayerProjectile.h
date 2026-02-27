#pragma once

#include "WEngine/Entity.h"
#include <string>


class WE::WVec2;

class PlayerProjectile : public WE::Entity
{
public:
	PlayerProjectile()
	{
		initialSize = WE::WVec2(2);
	}

	void Start() override;
	void Update(float deltaTime) override;
	
	void On_CollisionBegin(Entity* other, WE::WVec2 hitLocation) override;
	void On_EnterOtherSensor(Entity* otherSensor) override;

protected:
	virtual void OnDestroy(WE::WVec2 location);

protected:
	float lifeTime = 1.7f;
	float projectileSpeed = 45.f;
	float projectileDmg = 25.f;

	std::string filePath = "graphics/missile.bmp";

	int hTiles = 2;
	int vTiles = 3;
	int tileOffset = 0;
	int tileSpan = 2;
	int layer = -1;

	float animFps = 5.f;

	float soundVolume = 0.15f;
	std::string shootSounds[5]{
	"sounds/Explosion_1.5.wav",
	"sounds/Explosion_1.4.wav",
	"sounds/Explosion_1.1.wav",
	"sounds/Explosion_1.2.wav",
	"sounds/Explosion_1.3.wav" };

};

