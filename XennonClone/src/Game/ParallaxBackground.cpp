#include "ParallaxBackground.h"
#include "WEngine/GameContext.h"


using namespace WE;

void ParallaxBackground::Start()
{
	Entity::Start();

	//GetGameContext()->RENDER_AddRenderComponent(this, filePath, 1, 1, 0, 1 * 1, layer, WE::WVec2(0));
	//GetGameContext()->RENDER_SetAnimationParameters(this, false, 0.f);


	CreateTiles();
	
	

}

void ParallaxBackground::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	for (Entity* tile : tileEntities)
	{
		WVec2 newPos = tile->GetLocation() + WVec2(0, -scrollVel * deltaTime);

		if (newPos.y < -30)
			newPos.y = 30;

		tile->SetLocation(newPos);

	}

}




void ParallaxBackground::CreateTiles()
{
	//right side
	{
		Entity* tile = GetGameContext()->GAME_InstantiateEntity<Entity>(WVec2(4, 0) * tileSizeMult, WVec2(1) * tileSizeMult);
		GetGameContext()->RENDER_AddRenderComponent(tile, filePath, 8, 32, 180, 1, layer);
		tileEntities.push_back(tile);
	}

	{
		Entity* tile = GetGameContext()->GAME_InstantiateEntity<Entity>(WVec2(3, 0) * tileSizeMult, WVec2(1) * tileSizeMult);
		GetGameContext()->RENDER_AddRenderComponent(tile, filePath, 8, 32, 179, 1, layer);
		tileEntities.push_back(tile);
	}

	{
		Entity* tile = GetGameContext()->GAME_InstantiateEntity<Entity>(WVec2(2, 0) * tileSizeMult, WVec2(1) * tileSizeMult);
		GetGameContext()->RENDER_AddRenderComponent(tile, filePath, 8, 32, 178, 1, layer);
		tileEntities.push_back(tile);
	}

	{
		Entity* tile = GetGameContext()->GAME_InstantiateEntity<Entity>(WVec2(1, 0) * tileSizeMult, WVec2(1) * tileSizeMult);
		GetGameContext()->RENDER_AddRenderComponent(tile, filePath, 8, 32, 177, 1, layer);
		tileEntities.push_back(tile);
	}


	//left side
	{
		Entity* tile = GetGameContext()->GAME_InstantiateEntity<Entity>(WVec2(-3, 3) * tileSizeMult, WVec2(1) * tileSizeMult);
		GetGameContext()->RENDER_AddRenderComponent(tile, filePath, 8, 32, 192, 1, layer);
		tileEntities.push_back(tile);
	}
	{
		Entity* tile = GetGameContext()->GAME_InstantiateEntity<Entity>(WVec2(-2, 3) * tileSizeMult, WVec2(1) * tileSizeMult);
		GetGameContext()->RENDER_AddRenderComponent(tile, filePath, 8, 32, 193, 1, layer);
		tileEntities.push_back(tile);
	}
	{
		Entity* tile = GetGameContext()->GAME_InstantiateEntity<Entity>(WVec2(-1, 3) * tileSizeMult, WVec2(1) * tileSizeMult);
		GetGameContext()->RENDER_AddRenderComponent(tile, filePath, 8, 32, 194, 1, layer);
		tileEntities.push_back(tile);
	}

	//left side
	{
		Entity* tile = GetGameContext()->GAME_InstantiateEntity<Entity>(WVec2(4, -8) * tileSizeMult, WVec2(1) * tileSizeMult);
		GetGameContext()->RENDER_AddRenderComponent(tile, filePath, 8, 32, 194, 1, layer);
		tileEntities.push_back(tile);
	}
	{
		Entity* tile = GetGameContext()->GAME_InstantiateEntity<Entity>(WVec2(3, -8) * tileSizeMult, WVec2(1) * tileSizeMult);
		GetGameContext()->RENDER_AddRenderComponent(tile, filePath, 8, 32, 193, 1, layer);
		tileEntities.push_back(tile);
	}
	{
		Entity* tile = GetGameContext()->GAME_InstantiateEntity<Entity>(WVec2(2, -8) * tileSizeMult, WVec2(1) * tileSizeMult);
		GetGameContext()->RENDER_AddRenderComponent(tile, filePath, 8, 32, 192, 1, layer);
		tileEntities.push_back(tile);
	}



}
