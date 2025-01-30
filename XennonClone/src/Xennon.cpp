#include <WEngine.h>

#include "Game/XennonLevel.h"

class Xennon : public WE::Game
{

public:
	Xennon()
	{
		windowWidth = 600;
		windowHeight = 800;
		gameName = "Xennon Clone";
	}

	~Xennon()
	{

	}

	
	void Start() override
	{
		Game::Start();
		
		XennonLevel* level = gameContext->GAME_InstantiateEntity<XennonLevel>(WE::WVec2(0, 0));
		
	}

};


WE::Game* WE::CreateGame()
{
	return new Xennon();
}