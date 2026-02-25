#pragma once
#include "PlayerProjectile.h"
#include "WEngine/GameContext.h"
#include "Explosion2.h"
class PlayerProjectile2 : public PlayerProjectile
{
public:
	PlayerProjectile2()
	{
		lifeTime = 1.7f;
		projectileSpeed = 45.f;
		projectileDmg *= 2;

		std::string filePath = "graphics/missile.bmp";

		hTiles = 2;
		vTiles = 3;
		tileOffset = 2;
		tileSpan = 2;
		layer = -1;

		animFps = 7.5f;
	}

protected:
	void OnDestroy(WE::WVec2 location) override
	{
		GetGameContext()->GAME_InstantiateEntity<Explosion2>(location, -6.28f / 3, WE::WVec2(2.3f));
	}

};

