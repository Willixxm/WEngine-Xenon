#include "GameContext.h"
#include "Game.h"
#include "WMath.h"
#include "Entity.h"

#include <memory>
#include <string>
#include <iostream>

#include <SDL.h>
#include <SDL_events.h>
#include <SDL_joystick.h>
#include <box2d.h>



namespace WE
{


	class GameContext::SDLWindow
	{
	public:
		SDLWindow(std::string wName, unsigned int wWidth, unsigned int wHeight) : windowName{ wName }, windowWidth{ wWidth }, windowHeight{ wHeight }
		{
			// INIT SDL
			if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
				std::cerr << "failed to initialize video" << std::endl;

			SDL_JoystickEventState(true);
			
			OpenJoyStick();



			window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);

			if (window == nullptr)
				std::cout << "failed to create window" << std::endl;

			renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

			if (renderTarget == nullptr)
				std::cout << "failed to create renderTarget" << std::endl;

		}

		~SDLWindow()
		{
			SDL_DestroyWindow(window);
			window = nullptr;
			SDL_DestroyRenderer(renderTarget);
			renderTarget = nullptr;

			if (playerJoyStick)
				SDL_JoystickClose(playerJoyStick);
			

			SDL_Quit();
		}

		void UpdateEvents()
		{
			userInputs.mouseInputVector = WVec2(0);
			for (float value : userInputs.joyAxis)
				value = 0.f;


			while (SDL_PollEvent(&windowEvent))
			{
				switch (windowEvent.type)
				{	
					case SDL_QUIT:
						stopGame = true;
						break;

						
					case SDL_KEYUP:
					case SDL_KEYDOWN:
						switch (windowEvent.key.keysym.sym)
						{
						case SDLK_w:
							userInputs.keys[INPUT_KeyCode::Key_W] = windowEvent.key.state; 
							break;
						case SDLK_a:
							userInputs.keys[INPUT_KeyCode::Key_A] = windowEvent.key.state;
							break;
						case SDLK_s:
							userInputs.keys[INPUT_KeyCode::Key_S] = windowEvent.key.state;
							break;
						case SDLK_d:
							userInputs.keys[INPUT_KeyCode::Key_D] = windowEvent.key.state;
							break;
						}
						break;


					case SDL_MOUSEBUTTONDOWN:
					case SDL_MOUSEBUTTONUP:
						switch (windowEvent.button.button)
						{
						case 1:
							userInputs.keys[INPUT_KeyCode::Key_MouseLeft] = windowEvent.button.state;
							break;
						case 2:
							userInputs.keys[INPUT_KeyCode::Key_MouseRight] = windowEvent.button.state;
							break;
						}
						break;

					
					case SDL_MOUSEMOTION:
						userInputs.mouseInputVector.x += windowEvent.motion.xrel;
						userInputs.mouseInputVector.y += windowEvent.motion.yrel;
						break;




					case SDL_JOYAXISMOTION:
						if (windowEvent.jaxis.which < 12)
							userInputs.joyAxis[windowEvent.jaxis.which] = (float)windowEvent.jaxis.value / maxStickValue;
						else
							std::cerr << "axis index exceeded\n";
						break;

					case SDL_JOYBUTTONDOWN:
					case SDL_JOYBUTTONUP:
						if (windowEvent.jbutton.button < 24)
							userInputs.joyButtons[windowEvent.jbutton.button] = windowEvent.jbutton.state;
						else
							std::cerr << "button index exceeded\n";
						break;

					case SDL_JOYDEVICEADDED:
						OpenJoyStick();
						break;
					case SDL_JOYDEVICEREMOVED:
						if (windowEvent.jdevice.which == 0)
							SDL_JoystickClose(playerJoyStick);
						playerJoyStick = nullptr;
						std::cout << "Controller disconnected\n";
						OpenJoyStick();
						break;

				} 
			}
		}

		void RenderFrame()
		{

		}

		void OpenJoyStick()
		{
			if (SDL_NumJoysticks() && playerJoyStick == nullptr)
			{
				playerJoyStick = SDL_JoystickOpen(0);
				std::cout << SDL_JoystickNameForIndex(0) << " connected\n";
			}
		}

		UserInputs* GetUserInputs()
		{
			return &userInputs;
		}

	private:
		SDL_Window* window = nullptr;
		SDL_Renderer* renderTarget = nullptr;
		std::string windowName;
		unsigned int windowWidth;
		unsigned int windowHeight;

		SDL_Event windowEvent = SDL_Event();
		UserInputs userInputs;

		SDL_Joystick* playerJoyStick;

		short int maxStickValue = 32767;
		short int minStickValue = -32768;
		short int leftStickDeadZone = maxStickValue * 0.1;
		short int rightStickDeadZone = maxStickValue * 0.1;

	public:
		bool stopGame = false;

	};

	class GameContext::Box2D
	{
	public:
		Box2D()
		{
			CreateWorld();
		}

		~Box2D()
		{
			b2DestroyWorld(worldId);
		}

		void UpdatePhysicsWorld(float deltaTime)
		{
			b2World_Step(worldId, deltaTime, 4);
		}

		b2BodyId GetB2IdFromWId(WPhysBodyId WId)
		{
			b2BodyId b2Id;
			b2Id.index1 = WId.physBodyId;
			b2Id.world0 = WId.physWorldId;
			b2Id.generation = WId.physGeneration;

			return b2Id;
		}

		WVec2 GetObjPos(WPhysBodyId id)
		{
			b2Vec2 position = b2Body_GetPosition(GetB2IdFromWId(id));

			return WVec2(position.x, position.y);
		}

		void SetObjPos(WPhysBodyId id, WVec2 pos)
		{
			b2BodyId b2Id = GetB2IdFromWId(id);
			b2Vec2 position;
			position.x = pos.x;
			position.y = pos.y;
			b2Rot rotation = b2Body_GetRotation(b2Id);

			b2Body_SetTransform(b2Id, position, rotation);
		}

		void CreateWorld()
		{
			b2WorldDef worldDef = b2DefaultWorldDef();
			worldDef.gravity = { 0.f, 0.f };
			worldId = b2CreateWorld(&worldDef);
		}

		void SetWorldGravity(WVec2 vec)
		{
			b2Vec2 gravity;
			gravity.x = vec.x;
			gravity.y = vec.y;

			b2World_SetGravity(worldId, gravity);
		}

		WPhysBodyId CreateObj(WVec2 pos, WBodyType bodyType, WVec2 boxSize)
		{
			b2BodyDef bodyDef = b2DefaultBodyDef();
			bodyDef.type = (b2BodyType)bodyType;
			bodyDef.position = { pos.x, pos.y};
			bodyDef.fixedRotation = true;
			b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

			b2Polygon bodyBox = b2MakeBox(boxSize.x/2, boxSize.y/2);
			b2ShapeDef boxShapeDef = b2DefaultShapeDef();
			boxShapeDef.density = 1.f;
			//boxShapeDef.friction = 0.3f;

			b2CreatePolygonShape(bodyId, &boxShapeDef, &bodyBox);

			return WPhysBodyId(bodyId.index1, bodyId.world0, bodyId.generation);
		}

		void DestroyObj(WPhysBodyId id)
		{
			b2DestroyBody(GetB2IdFromWId(id));
		}


	private:
		b2WorldId worldId;


	};



	GameContext::GameContext(Game* game) : game{ game }, windowPImpl { std::make_unique<SDLWindow>(game->gameName, game->windowWidth, game->windowHeight) }, physicsPImpl{ std::make_unique<Box2D>() }
	{
		
	}

	GameContext::~GameContext()
	{

	}

	void GameContext::SYSTEM_UpdatePhysics(float deltaTime)
	{
		physicsPImpl->UpdatePhysicsWorld(deltaTime);
		for (size_t i = 0; i < instancedEntities.size(); ++i)
		{
			if(instancedEntities[i]->bodyId.isValid)
				instancedEntities[i]->SetLocation(physicsPImpl->GetObjPos(instancedEntities[i]->bodyId));
		}
	}

	void GameContext::SYSTEM_UpdateEventSystem()
	{
		windowPImpl->UpdateEvents();
		if (windowPImpl->stopGame)
			game->stopGame = true;
			
	}

	void GameContext::SYSTEM_UpdateEntities(float deltaTime)
	{
		for (size_t i = 0; i < instancedEntities.size(); ++i)
		{
			instancedEntities[i]->Update(deltaTime);
		}
	}

	void GameContext::SYSTEM_Render()
	{
		windowPImpl->RenderFrame();
	}


	bool GameContext::INPUT_GetKeyDown(INPUT_KeyCode keyCode)
	{
		return windowPImpl->GetUserInputs()->keys[keyCode];
	}

	WVec2 GameContext::INPUT_GetMouseInput()
	{
		return windowPImpl->GetUserInputs()->mouseInputVector;
	}

	float GameContext::INPUT_GetJoyAxisValue(INPUT_JoyAxisCode axisCode)
	{
		return windowPImpl->GetUserInputs()->joyAxis[axisCode];
	}

	bool GameContext::INPUT_GetJoyButtonDown(INPUT_JoyButtonCode buttonCode)
	{
		return windowPImpl->GetUserInputs()->joyButtons[buttonCode];
	}


	void GameContext::GAME_AddPhysComponentToEntity(Entity* entity, WBodyType bodyType, WVec2 size)
	{
		if (!entity->bodyId.isValid)
			entity->bodyId = physicsPImpl->CreateObj(entity->GetLocation(), bodyType, size);
	}


	WVec2 GameContext::PHYS_GetLocationOfPhysObj(WPhysBodyId id)
	{
		return physicsPImpl->GetObjPos(id);
	}

	void GameContext::PHYS_SetLocationOnPhysObj(WPhysBodyId id, WVec2 pos)
	{
		physicsPImpl->SetObjPos(id, pos);
	}

	void GameContext::GAME_SetWorldGravity(WVec2 vec)
	{
		physicsPImpl->SetWorldGravity(vec);
	}

	void GameContext::On_InstantiateEntity(Entity* entity, WVec2 pos)
	{
		entity->gameContext = this;
		entity->SetLocation(pos);

		instancedEntities.push_back(entity);

		entity->Start();
	}
	
	void GameContext::GAME_DestroyEntity(Entity* entity)
	{
		for (size_t i = 0; i < instancedEntities.size(); ++i)
		{
			if (instancedEntities[i] != entity)
				continue;
			
			if(instancedEntities[i]->bodyId.isValid)
				physicsPImpl->DestroyObj(instancedEntities[i]->bodyId);
			

			delete instancedEntities[i];
			instancedEntities[i] = nullptr;
			instancedEntities.erase(instancedEntities.begin() + i);

		}
	}

}