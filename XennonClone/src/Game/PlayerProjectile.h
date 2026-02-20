#pragma once

#include "WEngine/Entity.h"


class WE::WVec2;

class PlayerProjectile : public WE::Entity
{
public:

	void Start() override;
	void Update(float deltaTime) override;
	
	void On_CollisionBegin(Entity* other, WE::WVec2 hitLocation) override;
	void On_EnterOtherSensor(Entity* otherSensor) override;

private:
	float lifeTime = 3.f;
	float projectileSpeed = 30.f;
	float projectileDmg = 25.f;



};

