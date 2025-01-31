#pragma once

#include "Core.h"
#include "WMath.h"
#include <vector>

namespace WE
{
	class GameContext;

	class WPhysBodyId
	{
	public:

		WPhysBodyId(unsigned int bodyId, unsigned int worldId, unsigned int gen) : physBodyId{ bodyId }, physWorldId{ worldId }, physGeneration{ gen } {}
		WPhysBodyId() : WPhysBodyId(0, 0, 0) { isValid = false; }

		unsigned int physBodyId = 0;
		unsigned int physWorldId = 0;
		unsigned int physGeneration = 0;

		bool isValid = true;
	};

	class WE_API Entity
	{

	public:
		Entity();
		virtual ~Entity();


	public:
		WPhysBodyId bodyId;
	private:
		WVec2 position = WVec2(0.f);
		WVec2 initialSize = WVec2(0.f);
		int renderLayer = 0; 

		float aliveTime = 0;

	public:
		WVec2 GetLocation();
		void SetLocation(WVec2 pos);

		WVec2 GetInitialSize() const { return initialSize; }

		float GetTimeAlive() const { return aliveTime; }

	private:
		GameContext* gameContext = nullptr;

	protected:
		GameContext* GetGameContext() const { return gameContext; }

	public:

		virtual void Start();
		virtual void Update(float deltaTime);


		void Destroy();

	public:

		virtual void On_CollisionBegin(Entity* other, WVec2 point);

		friend class GameContext; //to access and set gameContext variable on instantiation
	};
}



