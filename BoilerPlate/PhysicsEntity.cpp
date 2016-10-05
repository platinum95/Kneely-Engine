#include "PhysicsEntity.h"
#include "PhysicsConstants.h"
#include "glm/gtx/transform.hpp"

namespace BoilerPlate {
	namespace Physics {

		PhysicsEntity::PhysicsEntity(glm::vec3 pos, glm::vec3 rot, float fl) {
			velocity = glm::vec3(0, 0, 0);
			position = glm::vec3(0, 0, 0);
			forces = glm::vec3(0, 0, 0);
			rotation = glm::vec3(0, 0, 0);
			rotationMat = glm::mat4(1.0);
			mass = 20;
		}

		PhysicsEntity::PhysicsEntity(glm::vec3 pos, glm::vec3 rot){
			velocity = glm::vec3(0, 0, 0);
			position = pos;
			forces = glm::vec3(0, 0, 0);
			rotation = rot;
			rotationMat = glm::mat4(1.0);
			mass = 20;
		}
		PhysicsEntity::PhysicsEntity(){
			velocity = glm::vec3(0, 0, 0);
			position = glm::vec3(0, 0, 0);
			forces = glm::vec3(0, 0, 0);
			rotation = glm::vec3(0, 0, 0);
			rotationMat = glm::mat4(1.0);

			mass = 20;
		}

		void PhysicsEntity::rotate(glm::vec3 axis, float angle) {
			rotationMat = glm::rotate(angle, axis);
			right = rotationMat[0];
			up = rotationMat[1];
			forward = rotationMat[2];
//			glm::mat4 currentBasis = glm::mat4(glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(0, 0, 0, 0));
//			glm::mat4 newBasis = rotation * currentBasis;
//			newBasis.

		}

		void PhysicsEntity::ChangeBasis(glm::vec3 _right, glm::vec3 _up, glm::vec3 _forward) {
			rotationMat = glm::mat4(glm::vec4(_right, 0), glm::vec4(_up, 0),  glm::vec4(_forward, 0), glm::vec4(0, 0, 0, 1));
		}

		void PhysicsEntity::addForce(glm::vec3 _forces) {
			this->forces += _forces;
		}

		PhysicsEntity::PhysicsEntity(int i){
		}

		void PhysicsEntity::update(std::vector<PhysicsEntity*> eList, float) {

		}


		
		void StaticEntity::update(std::vector<PhysicsEntity*> eList, float timeDiff) {



		}





		DynamicEntity::DynamicEntity(glm::vec3 pos, glm::vec3 rot) : BoilerPlate::Physics::PhysicsEntity(pos, rot) {
			velocity = glm::vec3(0, 0, 0);
			position = glm::vec3(0, 0, 0);
			forces = glm::vec3(0, 0, 0);
			rotation = glm::vec3(0, 0, 0);
			mass = 20;
		}
		DynamicEntity::DynamicEntity() {
			velocity = glm::vec3(0, 0, 0);
			position = glm::vec3(0, 0, 0);
			forces = glm::vec3(0, 0, 0);
			rotation = glm::vec3(0, 0, 0);
			mass = 20;
		}


		void DynamicEntity::update(float timeDiff) {
			//Add gravity vector
			this->addForce(glm::vec3(0, (PhysicsConstants::gravity * this->mass), 0));

			//Get and store the current speed
			float currentSpeed = glm::length(this->velocity);

			//Do wind resistance stuff
			glm::vec3 windVelocityUnit = -(this->velocity);
			glm::vec3 windVelocity;

			if (currentSpeed > 0.5)
				windVelocity = 1.0f/2.0f * 1.225f * squareKeepSign(windVelocityUnit) * PhysicsConstants::drag_coef;
			else if (currentSpeed > 0.01)
				windVelocity = windVelocityUnit * PhysicsConstants::drag_coef;
			else
				windVelocity = glm::vec3(0, 0, 0);

			//Calculate accelerations
			glm::vec3 accelerations = this->forces * (1 / this->mass);
			
			//Get the new velocity from accelerations and wind
			this->velocity += (accelerations * timeDiff);
			this->velocity += windVelocity;

			//Get the new position
			this->position += (this->velocity * timeDiff);

			/*
			if (this->position.y < PhysicsConstants::world->GetHeight(this->position.x, this->position.z)) {
				this->velocity.y = 0;
				this->position.y = PhysicsConstants::world->GetHeight(this->position.x, this->position.z);

			}
			*/

			//Reset the force vector
			this->forces = glm::vec3(0, 0, 0);

		}
		glm::vec3 DynamicEntity::squareKeepSign(glm::vec3 in){
			float x = (in.x >= 0 ? 1 : -1) * in.x* in.x;
			float y = (in.y >= 0 ? 1 : -1) * in.y* in.y;
			float z = (in.z >= 0 ? 1 : -1) * in.z* in.z;
			return glm::vec3(x, y, z);
		}

	}
}
