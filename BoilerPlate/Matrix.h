#pragma once

#include "Vector.h"
#include <cmath>
namespace BoilerPlate {
	namespace Maths {

		class mat2 {
		
		};

		class mat3 {

		};

		class mat4 {
		public:
			mat4();
			float	m00, m01, m02, m03,
					m10, m11, m12, m13,
					m20, m21, m22, m23,
					m30, m31, m32, m33;
			float* toData();
			void setIdentity();
			float& operator[](int i);
			mat4 operator*( mat4& rhs);
			mat4 RotationX(float rad);

			mat4 RotationY(float rad);

			mat4 RotationZ(float rad);

			static mat4 rotate(float angle, vec3 axis, mat4 src, mat4 *dest);
			static mat4 translate(vec3 vec, mat4 src, mat4* dest);


		};

		class matN {

		};
	}
}
