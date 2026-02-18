#include <WEngine.h>

#include "Game/XennonLevel.h"

class Xennon : public WE::Game
{

public:
	Xennon()
	{
		windowWidth = 500;
		windowHeight = windowWidth * 4/3;
		gameName = "Xennon Clone";
		renderEngine = WRenderEngine::SDL;
	}

	~Xennon()
	{

	}

	
	void Start() override
	{
		Game::Start();
		
		XennonLevel* level = gameContext->GAME_InstantiateEntity<XennonLevel>(WE::WVec2(0, 0), WE::WVec2(0, 0));	
	}
};


WE::Game* WE::CreateGame()
{
	return new Xennon();
}