#pragma once

#include "Core.h"
#include <memory>
#include <vector>
#include <string>
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
		LeftStick_Horizontal  = 0,
		LeftStick_Vertical    = 1,
		RightStick_Horizontal = 2,
		RightStick_Vertical   = 3

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

	enum WE_API WRenderType : int
	{
		Render_Surface = 0,
		Render_Texture = 1,
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
		void SYSTEM_Render(float deltaTime);

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

		void PHYS_AddPhysComponentToEntity(Entity*, WBodyType bodyType, WVec2 size);
		void PHYS_SetLocationOnPhysObj(WPhysBodyId id, WVec2 pos);
		void PHYS_SetLinearVelocityOnPhysObj(WPhysBodyId id, WVec2 vel);

		WVec2 PHYS_GetLocationOfPhysObj(WPhysBodyId id);

	public:

		void RENDER_AddRenderComponent(Entity* entity, std::string filePath, int hTiles, int vTiles, float width, float height, WRenderType renderType, int layer);
		void RENDER_RemoveRenderComponent(Entity* entity, WRenderType renderType);

		void RENDER_SetAnimationParameters(Entity* entity, bool autoAnimate, float animationFps);
		void RENDER_SetManualAnimationState(Entity* entity, float state_0to1);

	public:
		void PHYS_SetWorldGravity(WVec2);
	
	};
}


