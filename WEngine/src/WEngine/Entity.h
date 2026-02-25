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

		WPhysBodyId(unsigned int bodyId, unsigned int worldId, unsigned int gen) : physBodyId{ bodyId }, physWorldId{ worldId }, physGeneration{ gen }, isValid{ true } {}
		WPhysBodyId() : WPhysBodyId(0, 0, 0) { isValid = false; }

		unsigned int physBodyId = 0;
		unsigned int physWorldId = 0;
		unsigned int physGeneration = 0;

		bool isValid = false;
	};

	class WE_API Entity
	{

	public:
		Entity();
		virtual ~Entity();

		uint32_t GetID() const { return entityID; }
	

	public:
		WPhysBodyId bodyId;

	protected:
		WVec2 initialSize = WVec2(1.f);
	private:
		WVec2 position = WVec2(0.f);
		float rotationRad = 0.f;
		int renderLayer = 0; 

		float aliveTime = 0;

		bool isValid = true;
		uint32_t entityID = 0;

	public:
		WVec2 GetLocation() const { return position; }
		virtual void SetLocation(WVec2 pos);

		float GetRotation() const { return rotationRad; }
		virtual void SetRotation(float newRotationRad);

		WVec2 GetLinearVelocity();

		WVec2 GetUpVector();
		WVec2 GetRightVector();

		WVec2 GetInitialSize() const { return initialSize; }

		float GetTimeAlive() const { return aliveTime; }

	private:
		GameContext* gameContext = nullptr;

	protected:
		GameContext* GetGameContext() const { return gameContext; }

	public:

		virtual void Start();
		virtual void Update(float deltaTime);

		virtual void OnCoroutineUpdate(int CoroutineID, float CoroutineTime) {}
		virtual void OnCoroutineEnd(int CoroutineID) {}


		virtual void Destroy();

	public:

		virtual void On_CollisionBegin(Entity* other, WVec2 point);
		virtual void On_SensorBeginOverlap(Entity* other);
		virtual void On_EnterOtherSensor(Entity* otherSensor);//UNUSED

		friend class GameContext; //to access and set gameContext variable on instantiation
	};
}



