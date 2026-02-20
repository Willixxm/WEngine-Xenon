#include "ParallaxBackground.h"
#include "WEngine/GameContext.h"
#include <cmath>

using namespace WE;

void ParallaxBackground::Start()
{
	Entity::Start();

	CreateTiles();
}

void ParallaxBackground::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	auto UpVec = GetUpVector();

	for (Entity* tile : tileEntities_close)
	{
		WVec2 newPos = tile->GetLocation() + UpVec * -scrollVel_close * deltaTime;

		if (newPos.x < -64)
		{
			newPos.x += 2 * 64;
		}
		tile->SetLocation(newPos);
	}

	for (Entity* tile : tileEntities_far)
	{
		WVec2 newPos = tile->GetLocation() + UpVec * -scrollVel_far * deltaTime;

		if (newPos.x < -64)
		{
			newPos.x += 2*64;
		}
		tile->SetLocation(newPos);
	}
}

void ParallaxBackground::CreateTiles()
{
	std::vector<double> tileIDs;
	// -1: end line
	// 0: empty
	// left to right, down to up
	// int part -> start ID
	// decimal part (.00) -> span

	// =================
	// == close layer ==
	// =================
	int Xoffset = 0;
	int Yoffset = 0;

	tileIDs = { //facility left
		327.04, -1,
		273.04, -1,
		241.04, -1,
		160.05, -1,
		144.05, -1,
		128.07, -1,
		112.07, -1,
		96.07,  -1,
		80.05,  -1,
		64.06,  -1, 
		48.06,	-1,
		256.06, -1,
		240.05, -1,
		176.05, -1,
		160.03, 118, -1,
		176.03, 118, -1,
		160.03, 118, -1,
		176.03, 118, -1,

		145.04, -1,

		129.02, 224.04, -1,
		113.02, 208.04, 78.02, 110.02, 64+7, -1,
		97.02,  192.04, 62.02, 94.02,  48+7, -1,

		80.05,  -1,
		64.06,  -1,
		48.06,	-1,
		256.06, -1,
		240.05, -1,
		256.06, -1,
		240.05, -1,
		256.06, -1,
		240.05, -1,
		256.06, -1,
		288.05, -1,
		304+6.05, -1,		

		-1,-1,-1,-1,-1,

		327.04, -1,
		273.04, -1,
		241.04, -1,
		160.05, -1,
		144.05, -1,
		
		80.05,  -1,
		64.06,  -1,
		48.06,	-1,
		256.06, -1,
		240.05, -1,
		176.05, -1,
		160.03, 118, -1,
		176.03, 118, -1,
		160.03, 118, -1,
		176.03, 118, -1,

		145.04, 0.1, 150, 154.04, -1,
		129.02, 224.04, 0.06, 228.04, 128 + 11.02, -1,
		113.02, 208.04, 78.02, 110.02, 142.02, 208+4.04, 112 + 11.02,-1,
		97.02,  192.04, 62.02, 94.02, 126.02 , 192+4.04, 96 + 11.02,-1,

		80.05, 0.09, 149, 80+10.3, -1,
		64.06,  -1,
		48.06,	-1,
		256.06, -1,
		288.05, -1,
		304 + 6.05, -1
		
	};
	GenTileFromArray(tileIDs, Xoffset - 8, Yoffset + 0, layer_close, tileEntities_close);
 
	tileIDs = { //facility right
		0.06, 165.02, -1,
		0.06, 262.02, -1,
		0.05, 245.03, -1,
		0.05, 262.03, -1,
		0.04, 245.04, -1,
		0.04, 262.04, -1,
		0.03, 245.05, -1,
		0.03, 165.05, -1,
		0.01, 144 + 7.07, -1,
		0.01, 128 + 7.07, -1,
		0.01, 112 + 7.07, -1,
		0.01, 96 + 7.07, -1,
		0.01, 80 + 7.07, -1,
		0.03, 64 + 9.05, -1,
		0.03, 48 + 9.05, -1,
		0.03, 181.05, -1,
		0.04, 302.02, 178.02, -1,
		0.04, 286.02, 224 + 8.02, -1,
		0.04, 302.02, 208 + 8.02, -1,
		0.04, 286.02, 192 + 8.02, -1,
		320.11, -1,
		304.11, -1,

		//0.05, 496.05, -1,
		//0.05, 480.05, -1,
		0.05, 464.05, -1,
		0.05, 448.05, -1,
		0.05, 432.05, -1,
		0.05, 416.05, -1,
		0.05, 400.05, -1,

		//0.06, 496.05, -1,
		0.06, 480.05, -1,
		0.06, 464.05, -1,
		0.06, 448.05, -1,
		0.06, 432.05, -1,
		0.06, 416.05, -1,
		0.06, 400.05, -1,



	};
	GenTileFromArray(tileIDs, Xoffset + 1, Yoffset + 3, layer_close, tileEntities_close);

	tileIDs = { //second facility right
		 -1,
		 -1,
		 -1,
		 -1,
		0.04, 245.04, -1,
		0.04, 262.04, -1,
		0.03, 245.05, -1,
		0.03, 165.05, -1,
		//0.01, 144 + 7.07, -1, //connection to left incoming pipe
		//0.01, 128 + 7.07, -1,
		//0.01, 112 + 7.07, -1,
		//0.01, 96 + 7.07, -1,
		//0.01, 80 + 7.07, -1,
		-1,-1,-1, -1, -1,
		0.03, 64 + 9.05, -1,
		0.03, 48 + 9.05, -1,
		0.03, 181.05, -1,
		0.04, 302.02, 178.02, -1,
		0.04, 286.02, 224 + 8.02, -1,
		0.04, 302.02, 208 + 8.02, -1,
		0.04, 286.02, 192 + 8.02, -1,
		320.11, -1,
		304.11, -1
	};
	GenTileFromArray(tileIDs, Xoffset +  3, Yoffset + 46, layer_close, tileEntities_close);

	
	// =================
	// === far layer ===
	// =================
	Xoffset = 0;
	Yoffset = 0;

	tileIDs = {
		//691.06, 752.02, 756.04, -1,
		//691.06, 752.02, 756.04, -1,
		0.03, 784 + 8.06, -1,

		0.02, 784.06, 0.03, 784.06, -1,
		0.02, 768.06, 0.03, 768.06, -1,
		-1,
		0.04, 808.06, -1, 
		0.04, 792.06, -1,
		-1,

		0.05, 0.03, 784 + 8.06, -1, 
		-1,
		0.02, 0.02, 784.06, 0.03, 784.06, -1,
		0.02, 0.02, 768.06, 0.03, 768.06, -1,

		0.06, 808.06, -1,
		0.06, 792.06, -1,
		-1,-1,-1,
		
		0.01, 0.03, 784 + 8.06, -1,
		0.01, 0.02, 784.06, 0.03, 784.06, -1,
		0.01, 0.02, 768.06, 0.03, 768.06, -1,
		0.01, 808.06, -1,
		0.01, 792.06, -1,
		-1,-1,
		
		0.05, 0.03, 784 + 8.06, -1,
		0.05, 0.02, 784.06, 0.03, 784.06, -1,
		0.05, 0.02, 768.06, 0.03, 768.06, -1,
		0.05, 808.06, -1,
		0.05, 792.06, -1,
		-1,

		// ------

		0.03, 784 + 8.06, -1,

		0.02, 784.06, 0.03, 784.06, -1,
		0.02, 768.06, 0.03, 768.06, -1,
		-1,
		0.04, 808.06, -1,
		0.04, 792.06, -1,
		-1,

		0.05, 0.03, 784 + 8.06, -1,
		-1,
		0.02, 0.02, 784.06, 0.03, 784.06, -1,
		0.02, 0.02, 768.06, 0.03, 768.06, -1,

		0.06, 808.06, -1,
		0.06, 792.06, -1,
		-1,-1,-1,

		0.01, 0.03, 784 + 8.06, -1,
		0.01, 0.02, 784.06, 0.03, 784.06, -1,
		0.01, 0.02, 768.06, 0.03, 768.06, -1,
		0.01, 808.06, -1,
		0.01, 792.06, -1,
		-1,-1,

		0.05, 0.03, 784 + 8.06, -1,
		0.05, 0.02, 784.06, 0.03, 784.06, -1,
		0.05, 0.02, 768.06, 0.03, 768.06, -1,
		0.05, 808.06, -1,
		0.05, 792.06, -1,
		-1,

		// ---

		0.03, 784 + 8.06, -1,

		0.02, 784.06, 0.03, 784.06, -1,
		0.02, 768.06, 0.03, 768.06, -1,
		-1,
		0.04, 808.06, -1,
		0.04, 792.06, -1,
		-1,

		0.05, 0.03, 784 + 8.06, -1,
		-1,
		0.02, 0.02, 784.06, 0.03, 784.06, -1,
		0.02, 0.02, 768.06, 0.03, 768.06, -1,

		0.06, 808.06, -1,
		0.06, 792.06, -1,
		-1,-1,-1,

		0.01, 0.03, 784 + 8.06, -1,
		0.01, 0.02, 784.06, 0.03, 784.06, -1,
		0.01, 0.02, 768.06, 0.03, 768.06, -1,
		0.01, 808.06, -1,
		0.01, 792.06, -1,
		-1,-1,

		0.05, 0.03, 784 + 8.06, -1,
		0.05, 0.02, 784.06, 0.03, 784.06, -1,
		0.05, 0.02, 768.06, 0.03, 768.06, -1,
		0.05, 808.06, -1,
		0.05, 792.06, -1,
		-1,

		-1,
	};
	GenTileFromArray(tileIDs, Xoffset - 10, Yoffset -20, layer_far, tileEntities_far);

}

void ParallaxBackground::GenTileFromArray(const std::vector<double>& tileIDs, int x_offset, int y_offset, int layer, std::vector<WE::Entity*>& entities)
{
	int hTileCount = 16;
	int vTileCount = hTileCount * 4;

	int tx = x_offset;
	int ty = y_offset;

	auto refPos = GetLocation();

	// iterate over tileIDs
	for (unsigned int i = 0; i < tileIDs.size(); ++i)
	{
		if (tileIDs[i] == -1)
		{//new line
			tx = x_offset;
			++ty;
			continue;
		}

		int start = tileIDs[i];
		int span = (tileIDs[i] + 0.0001f) * 100.f;
		span = span % 100;

		if (span == 0)
			span = 1;

		for (unsigned int s = 0; s < span; ++s)
		{
			if (start != 0)
			{
				auto location = GetLocation() + GetRightVector() * GetInitialSize().x * tx + GetUpVector() * GetInitialSize().y * ty;
				Entity* tile = GetGameContext()->GAME_InstantiateEntity<Entity>(location, GetRotation(), GetInitialSize());
				GetGameContext()->RENDER_AddRenderComponent(tile, filePath, hTileCount, vTileCount, start + s, 1, layer);
				GetGameContext()->RENDER_SetAnimationParameters(tile, false);
				entities.push_back(tile);
			}
			++tx;
		}


	}
}
