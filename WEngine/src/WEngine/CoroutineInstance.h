#pragma once

#include "Entity.h"

namespace WE
{
	class CoroutineInstance : public Entity
	{
	public:
		CoroutineInstance(Entity* caller, uint32_t callerID, int coroutineID, float Duration);

		~CoroutineInstance();

		void Start() override;
		void Update(float deltaTime) override;


	private:
		float CoroutineDuration = 0;
		int CoroutineID;
		Entity* CoroutineCaller = nullptr;
		uint32_t CallerID = 0;



	};
}


