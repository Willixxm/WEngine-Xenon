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
			position = gameContext->PHYS_GetLocationOfPhysObj(bodyId);
		
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
}

