#pragma once

#include "Core.h"
#include "Entity.h"

namespace WE
{
	class WE_API Pawn : public Entity
	{
	public:

		void Start() override;
		void Update(float deltaTime) override;

	protected:

		void Move(WVec2 vector);

	public:
		virtual void DealDamage(float damage);

	};
}


