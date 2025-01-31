#include "Pawn.h"
#include "GameContext.h"

namespace WE
{

	void Pawn::Start()
	{
		Entity::Start();

	}

	void Pawn::Update(float deltaTime)
	{
		Entity::Update(deltaTime);


	}


	void Pawn::Move(WVec2 moveVector)
	{
		GetGameContext()->PHYS_SetLinearVelocityOnPhysObj(this->bodyId, moveVector);
	}

	void Pawn::DealDamage(float damage)
	{

	}
}