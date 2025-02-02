#pragma once

#include "WEngine/Entity.h"

class WE::WVec2;

class PlayerProjectile : public WE::Entity
{
public:

	void Start() override;
	void Update(float deltaTime) override;
	
	void On_SensorBeginOverlap(Entity* other) override;

private:
	float lifeTime = 2.f;
	float projectileSpeed = 20.f;
	float projectileDmg = 25.f;



};

