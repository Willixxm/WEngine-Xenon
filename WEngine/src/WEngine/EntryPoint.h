#pragma once


#ifdef WE_PLATFORM_WINDOWS


extern WE::Game* WE::CreateGame();

int main(int argc, char** argv)
{

	WE::Game* game = WE::CreateGame();

	game->Run();

	delete game;


	return 0;
}



#endif
