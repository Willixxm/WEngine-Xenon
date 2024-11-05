#include <WEngine.h>
#include <iostream>

class Xennon : public WE::Game
{

public:
	Xennon()
	{

	}

	~Xennon()
	{

	}

	void Run() override
	{
		std::cout << "xennon is running !" << std::endl;
		std::cin;
	}

};


WE::Game* WE::CreateGame()
{
	return new Xennon();
}