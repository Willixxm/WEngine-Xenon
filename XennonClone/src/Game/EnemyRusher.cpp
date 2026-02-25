#include "EnemyRusher.h"
#include "PowerUpWeapon.h"
//#include "PowerUpShield.h"

EnemyRusher::EnemyRusher()
{
	filePath = "graphics/rusher.bmp";
	hitboxSizeMult = 0.4f;
	hTiles = 4;
	vTiles = 6;
	tileOffset = 0;
	tileSpan = hTiles * vTiles;
	renderLayer = 0;
	animationFPS = 15.f;

	initialSize = WVec2(4);

	explosionSizeMult = 1.0f;

	bodyDamage = 50.f;

	lifePoints = 50.f;
	moveSpeed = 5.f;
	moveVector = WVec2(0, -1);
	
	maxLifeTime = 20.f;

	score = 10000;
}

void EnemyRusher::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	Move(moveVector * moveSpeed + WVec2(-3.3f, 0));
}

void EnemyRusher::DieByPlayer()
{
	short unsigned int random = (rand() % 100 + 1);
	if (random > 66)
		GetGameContext()->GAME_InstantiateEntity<PowerUpWeapon>(GetLocation(), 0.f);
	//else if (random > 80)
		//GetGameContext()->GAME_InstantiateEntity<PowerUpShield>(GetLocation(), 0.f);

	Enemy::DieByPlayer();
}