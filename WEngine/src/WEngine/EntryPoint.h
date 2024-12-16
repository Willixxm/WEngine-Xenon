#pragma once

#ifdef WE_PLATFORM_WINDOWS

extern WE::Game* WE::CreateGame();


int main(int argc, char** argv)
{
	WE::Game* game = WE::CreateGame();

	WE::WWindow* GameWindow = new WE::WWindow(game->GetWindowName(), game->GetWindowWidth(), game->GetWindowHeight());

	

	game->Start();

	while (true)
	{
		//event system

		//update entities

		//update physics 

		//render entities


	}

	delete GameWindow;
	delete game;


	return 0;
}



#endif
