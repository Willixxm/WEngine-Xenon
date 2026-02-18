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
		bool keys[10];
	};

	enum WE_API INPUT_JoyButtonCode : int
	{
		FaceDown = 0,
		FaceRight,
		FaceLeft,
		FaceUp

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
		MouseLeft = 0,
		MouseRight,
		W,
		A,
		S,
		D,
		Spacebar


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

	enum WE_API WCollisionLayer : uint32_t
	{
		Layer1 = 0x00000001,
		Layer2 = 0x00000002,
		Layer3 = 0x00000004,
		Layer4 = 0x00000008
	};

	enum WE_API WRenderEngine : bool
	{
		SDL = false,
		OpenGL = true
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
		std::vector<Entity*> entitiesToDestroyAfterUpdate;

	public:
		template< class T >
		T* GAME_InstantiateEntity(WVec2 pos, WVec2 size)
		{
			static_assert(std::is_base_of<Entity, T>::value, "T must be derived from Entity");
			
			T* newEntity = new T();

			On_InstantiateEntity(newEntity, pos, size);

			return newEntity;
		}

		void GAME_StartCoroutine(Entity* caller, int ID, float duration);

	private:
		void On_InstantiateEntity(Entity*, WVec2 pos, WVec2 size);
		void SYSTEM_DestroyAllEntities();
		void SYSTEM_DestroyAllEntities_QueuedForDestruction();

	public:
		void GAME_DestroyEntity(Entity*);


		void PHYS_SetWorldGravity(WVec2);

		void PHYS_AddPhysComponentToEntity(Entity* entity, WBodyType bodyType, WVec2 sizeOverride, uint32_t collisionLayer, uint32_t collidesWith, bool isSensor, float density);
		void PHYS_AddPhysComponentToEntity(Entity* entity, WBodyType bodyType); 
		void PHYS_SetLocationOnPhysObj(WPhysBodyId id, WVec2 pos);
		void PHYS_SetLinearVelocityOnPhysObj(WPhysBodyId id, WVec2 vel);

		WVec2 SYSTEM_GetLocationOfPhysObj(WPhysBodyId id);

	public:
		void RENDER_SetOrthoCameraSize(float size);
		void RENDER_SetOrthoCameraPosition(WVec2 newPos);

		void RENDER_AddRenderComponent(Entity* entity, std::string filePath, int hTiles, int vTiles, int tileOffset, int tileSpan, int layer, WVec2 sizeOverride);
		void RENDER_AddRenderComponent(Entity* entity, std::string filePath, int hTiles, int vTiles, int tileOffset, int tileSpan, int layer);

		void RENDER_RemoveRenderComponent(Entity* entity);

		void RENDER_SetAnimationParameters(Entity* entity, bool autoAnimate, float animationFps);
		void RENDER_SetAnimationTileParameters(Entity* entity, int tileOffset, int tileSpan);
		void RENDER_SetManualAnimationState(Entity* entity, float state_0to1);

	public:
		void MEMORY_SetAutoUnloadAssetIfUnused(bool enabled);
		
	
	};
}


