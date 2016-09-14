#pragma once

#include <vector>
#include "PhysicsEntity.h"
#include <glm/vec3.hpp>

namespace BoilerPlate {
	namespace Physics {
		class PhysicsWorld
		{
		public:
			PhysicsWorld();
			PhysicsWorld(float, float);
			~PhysicsWorld();
			std::vector<PhysicsEntity*> entityList;
			void addToWorld(PhysicsEntity* toAdd);
			void removeFromWorld(PhysicsEntity* toRem);
			void update();
			void update(float _timeDiff);

		private:
			float gravity;
			float drag_coef;
			std::vector<PhysicsEntity*> toAddQueue, toRemoveQueue;
			glm::vec3 gravVector;
			float prevTime;
			void CollisionDetection(std::vector<PhysicsEntity*> entities, int size, float timeDiff);
			bool locked;
			void lock();
			void unlock();
			void init();

		};
	}
}

