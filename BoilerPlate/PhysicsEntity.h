#pragma once
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <vector>


namespace BoilerPlate{
	namespace Physics {

		class PhysicsEntity{
		public:
			PhysicsEntity(glm::vec3, glm::vec3, float);
			PhysicsEntity(glm::vec3 pos, glm::vec3 rot);
			PhysicsEntity();
			void rotate(glm::vec3 axis, float angle);
			void ChangeBasis(glm::vec3 right, glm::vec3 up, glm::vec3 forward);
			PhysicsEntity(int i);

			void addForce(glm::vec3);
			virtual void update(std::vector<PhysicsEntity*> eList, float);
			float mass, restitution, friction;
			glm::vec3 velocity, forces, position, rotation, forward, up, right;
			glm::mat4 rotationMat;
		};
		
		class StaticEntity : public PhysicsEntity {

		public:
			void update(std::vector<PhysicsEntity*> eList, float);

		private:

		};

		class DynamicEntity : public PhysicsEntity {

		public:
			DynamicEntity(glm::vec3 pos, glm::vec3 rot);
			DynamicEntity();
			void update( float);
		private:
			glm::vec3 squareKeepSign(glm::vec3);

		};
		
	}
}

