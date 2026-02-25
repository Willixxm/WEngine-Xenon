#include "Asteroid.h"

Asteroid::Asteroid()
{
	filePath = "graphics/GAster96.bmp";
	hitboxSizeMult = 0.8f;
	hTiles = 5;
	vTiles = 5;
	tileOffset = 0;
	tileSpan = hTiles * vTiles;
	renderLayer = 0;
	animationFPS = 9.f;

	lifePoints = 200.f;
	isInvincible = false;

	maxLifeTime = 60.f;

	moveSpeed = 0.f;
}

void Asteroid::Start()
{
	Pawn::Start();

	GetGameContext()->PHYS_AddPhysComponentToEntity(this, WE::WBodyType::dynamicBody, GetInitialSize() * hitboxSizeMult, collisionLayer, collidesWith, isSensor, 20.f, true);
	GetGameContext()->RENDER_AddRenderComponent(this, filePath, hTiles, vTiles, tileOffset, tileSpan, renderLayer);
	GetGameContext()->RENDER_SetAnimationParameters(this, true, animationFPS);
}

void Asteroid::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	if (GetTimeAlive() < 2.f)
		Move(moveVector * moveSpeed);
}
