#pragma once
#include "Enemy.h"
#include <string>
class Asteroid : public Enemy
{
public:
	Asteroid();

	void Start() override;
	void Update(float deltaTime) override;

protected:
	std::string shootSounds[4]{
	"sounds/Explosion_3.1.wav",
	"sounds/Explosion_3.2.wav",
	"sounds/Explosion_3.3.wav",
	"sounds/Explosion_3.4.wav" };
};

