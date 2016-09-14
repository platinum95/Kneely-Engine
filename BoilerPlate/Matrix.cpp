#include "Matrix.h"


namespace BoilerPlate {
	namespace Maths {



		mat4::mat4() {
				m00 = m01 = m02 = m03 =
				m10 = m11 = m12 = m13 =
				m20 = m21 = m22 = m23 =
				m30 = m31 = m32 = m33 = 0;
		}

		float * mat4::toData() {
			float toReturn[16];
			toReturn[0] = m00;
			toReturn[1] = m01;
			toReturn[2] = m02;
			toReturn[3] = m03;
			toReturn[4] = m10;
			toReturn[5] = m11;
			toReturn[6] = m12;
			toReturn[7] = m13;
			toReturn[8] = m20;
			toReturn[9] = m21;
			toReturn[10] = m22;
			toReturn[11] = m23;
			toReturn[12] = m30;
			toReturn[13] = m31;
			toReturn[14] = m32;
			toReturn[15] = m33;
			return toReturn;
		}

		void mat4::setIdentity() {
			mat4();
			m00 = m11 = m22 = m33 = 1;
		}

		float& mat4::operator[](int i) {
			switch (i) {
			case 0:
				return this->m00;
				break;
			case 1:
				return this->m01;
				break;
			case 2:
				return this->m02;
				break;
			case 3:
				return this->m03;
				break;
			case 4:
				return this->m10;
				break;
			case 5:
				return this->m11;
				break;
			case 6:
				return this->m12;
				break;
			case 7:
				return this->m13;
				break;
			case 8:
				return this->m20;
				break;
			case 9:
				return this->m21;
				break;
			case 10:
				return this->m22;
				break;
			case 11:
				return this->m23;
				break;
			case 12:
				return this->m30;
				break;
			case 13:
				return this->m31;
				break;
			case 14:
				return this->m32;
				break;
			case 15:
				return this->m33;
				break;
			default:
				return this->m00;
				break;
			}
		}

		mat4 mat4::RotationX(float rad) {
			mat4 matrix;
			matrix.setIdentity();
			matrix.m11 = std::cos(rad);
			matrix.m21 = -std::sin(rad);
			matrix.m12 = std::sin(rad);
			matrix.m22 = std::cos(rad);
			return matrix;
		}

		mat4 mat4::RotationY(float rad) {
			mat4 matrix;
			matrix.setIdentity();
			matrix.m00 = std::cos(rad);
			matrix.m20 = -std::sin(rad);
			matrix.m02 = std::sin(rad);
			matrix.m22 = std::cos(rad);
			return matrix;
		}

		mat4 mat4::RotationZ(float rad) {
			mat4 matrix;
			matrix.setIdentity();
			matrix.m00 = std::cos(rad);
			matrix.m10 = -std::sin(rad);
			matrix.m01 = std::sin(rad);
			matrix.m11 = std::cos(rad);
			return matrix;
		}

		mat4 mat4::rotate(float angle, vec3 axis, mat4 src, mat4 *dest) {
			if (dest == NULL)
				dest = new mat4();

			float c = (float)cos(angle);
			float s = (float)sin(angle);
			float oneminusc = 1.0f - c;
			float xy = axis.x*axis.y;
			float yz = axis.y*axis.z;
			float xz = axis.x*axis.z;
			float xs = axis.x*s;
			float ys = axis.y*s;
			float zs = axis.z*s;

			float f00 = axis.x*axis.x*oneminusc + c;
			float f01 = xy*oneminusc + zs;
			float f02 = xz*oneminusc - ys;
			// n[3] not used
			float f10 = xy*oneminusc - zs;
			float f11 = axis.y*axis.y*oneminusc + c;
			float f12 = yz*oneminusc + xs;
			// n[7] not used
			float f20 = xz*oneminusc + ys;
			float f21 = yz*oneminusc - xs;
			float f22 = axis.z*axis.z*oneminusc + c;

			float t00 = src.m00 * f00 + src.m10 * f01 + src.m20 * f02;
			float t01 = src.m01 * f00 + src.m11 * f01 + src.m21 * f02;
			float t02 = src.m02 * f00 + src.m12 * f01 + src.m22 * f02;
			float t03 = src.m03 * f00 + src.m13 * f01 + src.m23 * f02;
			float t10 = src.m00 * f10 + src.m10 * f11 + src.m20 * f12;
			float t11 = src.m01 * f10 + src.m11 * f11 + src.m21 * f12;
			float t12 = src.m02 * f10 + src.m12 * f11 + src.m22 * f12;
			float t13 = src.m03 * f10 + src.m13 * f11 + src.m23 * f12;
			dest->m20 = src.m00 * f20 + src.m10 * f21 + src.m20 * f22;
			dest->m21 = src.m01 * f20 + src.m11 * f21 + src.m21 * f22;
			dest->m22 = src.m02 * f20 + src.m12 * f21 + src.m22 * f22;
			dest->m23 = src.m03 * f20 + src.m13 * f21 + src.m23 * f22;
			dest->m00 = t00;
			dest->m01 = t01;
			dest->m02 = t02;
			dest->m03 = t03;
			dest->m10 = t10;
			dest->m11 = t11;
			dest->m12 = t12;
			dest->m13 = t13;
			return *dest;
		}

		mat4 mat4::translate(vec3 vec, mat4 src, mat4 * dest) {
			if (dest == NULL)
				dest = new mat4();

			dest->m30 += src.m00 * vec.x + src.m10 * vec.y + src.m20 * vec.z;
			dest->m31 += src.m01 * vec.x + src.m11 * vec.y + src.m21 * vec.z;
			dest->m32 += src.m02 * vec.x + src.m12 * vec.y + src.m22 * vec.z;
			dest->m33 += src.m03 * vec.x + src.m13 * vec.y + src.m23 * vec.z;

			return *dest;
		}

		mat4 mat4::operator* ( mat4& rhs) {
			mat4 r = mat4();
			mat4 l = *this;
			int r_index = 0;
			for (int col = 0; col < 4; col++) {
				for (int row = 0; row < 4; row++) {
					float sum = 0.0f;
					for (int i = 0; i < 4; i++) {
						int rhsInt = i + col * 4;
						sum += rhs[rhsInt] * l[row + i * 4];
					}
					r[r_index] = sum;
					r_index++;
				}
			}
			return r;
		}

	}
}
