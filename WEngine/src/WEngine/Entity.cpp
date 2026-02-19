#include "Entity.h"
#include "GameContext.h"
#include "WMath.h"
#include <iostream>

namespace WE
{
	Entity::Entity()
	{
		
	}

	Entity::~Entity()
	{

	}

	void Entity::Start()
	{

	}

	void Entity::Update(float deltaTime)
	{
		if (bodyId.isValid)
			position = gameContext->SYSTEM_GetLocationOfPhysObj(bodyId);

		aliveTime += deltaTime;
		
	}

	void Entity::Destroy()
	{
		gameContext->GAME_DestroyEntity(this);
	}

	WVec2 Entity::GetLocation()
	{ 
		return position; 
	}

	void Entity::SetLocation(WVec2 pos)
	{
		if (bodyId.isValid)
			gameContext->PHYS_SetLocationOnPhysObj(bodyId, pos);
		
		position = pos;
	}

	float Entity::GetRotation()
	{
		return rotationRad;
	}

	void Entity::SetRotation(float newRotationRad)
	{
		if (bodyId.isValid)
			gameContext->PHYS_SetRotationOnPhysObj(bodyId, newRotationRad);

		rotationRad = newRotationRad;
	}

	WVec2 Entity::GetUpVector()
	{
		return WE::WVec2(cos(GetRotation() + 3.14159265 / 2), sin(GetRotation() + 3.14159265 / 2));
	}
	WVec2 Entity::GetRightVector()
	{
		return WE::WVec2(cos(GetRotation()), sin(GetRotation()));
	}

	void Entity::On_CollisionBegin(Entity* other, WVec2 point) {}

	void Entity::On_SensorBeginOverlap(Entity* other) {}
	void Entity::On_EnterOtherSensor(Entity* otherSensor) {} //UNUSED
	
}

