#include <WEngine.h>
#include <iostream>

class Xennon : public WE::Game
{

public:
	Xennon()
	{
		windowWidth = 400;
		windowHeight = 800;
		gameName = "Xennon Clone";
	}

	~Xennon()
	{

	}

	void Start() override
	{
		std::cout << "xennon is running !" << std::endl;
		std::cin;
	}

};


WE::Game* WE::CreateGame()
{
	return new Xennon();
}