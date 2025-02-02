#include "EnemyLoner.h"






EnemyLoner::EnemyLoner()
{
	
	filePath = "graphics/LonerA.bmp";
	hitboxSizeMult = 0.6f;
	hTiles = 4;
	vTiles = 4;
	tileOffset = 0;
	tileSpan = hTiles * vTiles;
	renderLayer = 0;
	animationFPS = 5.f;

	ySpeed = -3.f;
}



void EnemyLoner::Update(float deltaTime)
{
	Enemy::Update(deltaTime);

	Move(WVec2(sin(GetTimeAlive() * 4) * 3, ySpeed));

}






