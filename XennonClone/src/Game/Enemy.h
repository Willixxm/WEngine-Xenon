#pragma once

#include "WEngine/Pawn.h"

using namespace WE;

class Enemy : public Pawn
{

public:

	void Start() override;
	void Update(float deltaTime) override;

	void DealDamage(float damage) override;

};

