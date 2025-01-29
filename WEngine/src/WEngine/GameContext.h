#pragma once

#include "Core.h"
#include <memory>
#include <vector>
#include <type_traits>
#include "WMath.h"


namespace WE
{

	struct WE_API UserInputs
	{
		float joyAxis[12];
		bool joyButtons[24];

		WVec2 mouseInputVector = WVec2(0);
		bool keys[6];
	};

	enum WE_API INPUT_JoyButtonCode : int
	{
		ButtonFaceDown = 0,
		ButtonFaceRight = 1,
		ButtonFaceLeft = 2,
		ButtonFaceUp = 3

	};

	enum WE_API INPUT_JoyAxisCode : int
	{
		Axis_MouseX = 0,
		Axis_MouseY = 1

	};

	enum WE_API INPUT_KeyCode : int
	{
		Key_MouseLeft = 0,
		Key_MouseRight = 1,
		Key_W = 2,
		Key_A = 3,
		Key_S = 4,
		Key_D = 5

	};

	enum WE_API WBodyType : int
	{
		// zero mass, zero velocity, may be manually moved
		staticBody = 0,

		// zero mass, velocity set by user, moved by solver
		kinematicBody = 1,

		// positive mass, velocity determined by forces, moved by solver
		dynamicBody = 2,
	};


	class Game;
	class Entity;
	class WPhysBodyId;


	class WE_API GameContext
	{
	
	public:
		GameContext(Game* game);
		~GameContext();

	private:
		class SDLWindow;
		std::unique_ptr<SDLWindow> windowPImpl;

		class Box2D;
		std::unique_ptr<Box2D> physicsPImpl;

		Game* game;

	public:
		void SYSTEM_UpdatePhysics(float deltaTime);
		void SYSTEM_UpdateEventSystem();
		void SYSTEM_UpdateEntities(float deltaTime);
		void SYSTEM_Render();

		bool INPUT_GetKeyDown(INPUT_KeyCode);
		WVec2 INPUT_GetMouseInput();

		bool INPUT_GetJoyButtonDown(INPUT_JoyButtonCode);
		float INPUT_GetJoyAxisValue(INPUT_JoyAxisCode);


	private:
		std::vector<Entity*> instancedEntities;

	public:
		template< class T >
		T* GAME_InstantiateEntity(WVec2 pos)
		{
			static_assert(std::is_base_of<Entity, T>::value, "T must be derived from Entity");
			
			T* newEntity = new T();

			On_InstantiateEntity(newEntity, pos);

			return newEntity;
		}

		void On_InstantiateEntity(Entity*, WVec2 pos);
		void GAME_DestroyEntity(Entity*);

		void GAME_AddPhysComponentToEntity(Entity*, WBodyType bodyType, WVec2 size);
		void PHYS_SetLocationOnPhysObj(WPhysBodyId id, WVec2 pos);

		WVec2 PHYS_GetLocationOfPhysObj(WPhysBodyId id);

	public:
		void GAME_SetWorldGravity(WVec2);
	
	};
}


