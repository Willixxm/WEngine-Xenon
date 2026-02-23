#include "EnemyProjectile.h"
#include "WEngine/GameContext.h"
#include <cmath>

#include "Explosion2.h"
#include "XennonLevel.h"


EnemyProjectile::EnemyProjectile()
{
	float positionOffsetSign = ((float)std::signbit(GetLocation().x) - 0.5f) * 2;

	filePath = "graphics/EnWeap6.bmp";
	hitboxSizeMult = 0.6f;
	hTiles = 8;
	vTiles = 1;
	tileOffset = 0;
	tileSpan = hTiles * vTiles;
	renderLayer = 0;
	animationFPS = 10.f;

	lifePoints = 50.f;

	collidesWith = WCollisionLayer::Layer1 | WCollisionLayer::Layer2;
	maxLifeTime = 7.f;

	score = 1337;
}

void EnemyProjectile::Update(float deltaTime)
{
	Enemy::Update(deltaTime);
	HandleEnemyLifeTime();
}

void EnemyProjectile::DieByPlayer()
{
	GetGameContext()->GAME_InstantiateEntity<Explosion2>(GetLocation(), (rand() % 618) * 0.01f, GetInitialSize() * explosionSizeMult);

	if (GetLevelInstance())
	{
		GetLevelInstance()->AddScore(score);
	}

	auto scoreText = GetGameContext()->GAME_InstantiateEntity<TextRenderer>(GetLocation() + WVec2(0, 1), 0, WVec2(0.5));
	scoreText->SetText(std::to_string(score));
	scoreText->DestroyWithFloat(WVec2(0, 2), 0.5);

	Destroy();
}





