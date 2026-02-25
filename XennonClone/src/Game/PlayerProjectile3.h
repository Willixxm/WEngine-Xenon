#pragma once
#include "PlayerProjectile.h"
#include "WEngine/GameContext.h"
#include "Explosion2.h"
class PlayerProjectile3 : public PlayerProjectile
{
public:
	PlayerProjectile3()
	{
		lifeTime = 1.7f;
		projectileSpeed = 70.f;
		projectileDmg *= 3;

		std::string filePath = "graphics/missile.bmp";

		hTiles = 2;
		vTiles = 3;
		tileOffset = 4;
		tileSpan = 2;
		layer = -1;

		animFps = 10.f;
	}

protected:
	void OnDestroy(WE::WVec2 location) override
	{
		GetGameContext()->GAME_InstantiateEntity<Explosion>(location, -6.28f / 3, WE::WVec2(3.f));
	}

};

