#include <WEngine.h>

#include "Game/XennonLevel.h"

class Xennon : public WE::Game
{

public:
	Xennon()
	{
		windowWidth = 1600;
		windowHeight = windowWidth / ((float)16/7);
		gameName = "Xennon Clone";
		renderEngine = WRenderEngine::OpenGL;
	}

	~Xennon()
	{

	}

	
	void Start() override
	{
		Game::Start();
		
		XennonLevel* level = gameContext->GAME_InstantiateEntity<XennonLevel>(WE::WVec2(0, 0), 0.f, WE::WVec2(0, 0));	
	}
};


WE::Game* WE::CreateGame()
{
	return new Xennon();
}