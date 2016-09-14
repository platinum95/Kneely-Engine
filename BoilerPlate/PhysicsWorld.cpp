#include "PhysicsWorld.h"
#include "PhysicsConstants.h"
#include <algorithm>

namespace BoilerPlate {
	namespace Physics {
		PhysicsWorld::PhysicsWorld() {
			gravity = 9.81f;

			init();
		}


		PhysicsWorld::~PhysicsWorld() {

		}
		PhysicsWorld::PhysicsWorld(float grav, float drag) {		
			PhysicsConstants::gravity = grav;		
			PhysicsConstants::drag_coef = drag;

			init();
		}
		void PhysicsWorld::init() {
			entityList = std::vector<PhysicsEntity*>();
			prevTime = 0;
			gravVector = glm::vec3(0, gravity, 0);
			toAddQueue = std::vector<PhysicsEntity*>();
			toRemoveQueue = std::vector<PhysicsEntity*>();
			locked = false;
			
		}

		void PhysicsWorld::addToWorld(PhysicsEntity* toAdd) {
			if(!locked)
				entityList.push_back(toAdd);

			else {
				toAddQueue.push_back(toAdd);
			}
		}

		void PhysicsWorld::removeFromWorld(PhysicsEntity* toRem) {
			if (!locked)	
				entityList.erase(std::remove(entityList.begin(), entityList.end(), toRem), entityList.end());

			else {
				toRemoveQueue.push_back(toRem);
			}
		}
		void PhysicsWorld::lock() {
			locked = true;
		}

		void PhysicsWorld::unlock() {
			int addSize = toAddQueue.size(), remSize = toRemoveQueue.size();


			if (addSize != 0) {
				for (int i = 0; i < addSize; i++) {
					entityList.push_back(toAddQueue.at(i));
				}
			}
			if (remSize != 0) {
				for (int i = 0; i < addSize; i++) {
					entityList.erase(std::remove(entityList.begin(), entityList.end(), toRemoveQueue.at(i)), entityList.end());
				}
			}

			locked = false;
		}

		void PhysicsWorld::update(float _timeDiff) {
			lock();
			std::vector<PhysicsEntity*> entities = entityList;
			int size = entityList.size();
			

			CollisionDetection(entities, size, _timeDiff);
			
			

			unlock();
		}

		void PhysicsWorld::CollisionDetection(std::vector<PhysicsEntity*> entities, int size, float timeDiff) {
			/*
			EntityList checkedVectors;
			for (int i = 0; i < size; i++) {
				PhysicsEntity* currentEntity = entities.at(i);

				currentEntity->update(checkedVectors.getList(), timeDiff);
				

				checkedVectors.add(currentEntity);
			}
			*/
		}
	}
}
