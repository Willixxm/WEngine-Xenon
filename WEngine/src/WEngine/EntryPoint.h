#pragma once

#ifdef WE_PLATFORM_WINDOWS

extern WE::Game* WE::CreateGame();


int main(int argc, char** argv)
{
	
	auto game = WE::CreateGame();
	

	game->Start();

	while (!game->stopGame)
	{
		game->Update(); 
		
	}

	
	delete game;


	return 0;
}



#endif
