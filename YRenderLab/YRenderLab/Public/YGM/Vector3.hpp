#ifndef _YRENDER_YGM_VECTOR3_HPP_
#define _YRENDER_YGM_VECTOR3_HPP_

#include <Public/YGM/Vector.hpp>
#include <iostream>

namespace YRender {
	namespace YGM {
		template <typename T>
		class Vector<3, T> {
		private:
			using value_type = T;
		public:
			template<typename U, typename V, typename W>
			Vector(U x, V y, W z) :
				x(static_cast<value_type>(x)),
				y(static_cast<value_type>(y)),
				z(static_cast<value_type>(z)) {}
			explicit Vector(value_type val) : Vector(val, val, val) {}
			Vector() : Vector(static_cast<value_type>(0)) {}
			Vector(const Vector& rhs) : Vector(rhs.x, rhs.y, rhs.z) {}
			bool operator==(const Vector& rhs) {
				return Math::Equal(x, rhs.x) && Math::Equal(y, rhs.y) && Math::Equal(z, rhs.z);
			}
			bool operator!=(const Vector& rhs) const {
				return !Math::Equal(x, rhs.x) || !Math::Equal(y, rhs.y) || !Math::Equal(z, rhs.z);
			}
			Vector& operator=(const Vector& rhs) {
				x = rhs.x;
				y = rhs.y;
				z = rhs.z;
				return *this;
			}

			const Vector operator-(const Vector& rhs) const {
				return Vector(x - rhs.x, y - rhs.y, z - rhs.z);
			}

			const Vector operator+(const Vector& rhs) const {
				return Vector(x + rhs.x, y + rhs.y, z + rhs.z);
			}

			const Vector& operator-=(const Vector& rhs) {
				x -= rhs.x;
				y -= rhs.y;
				z -= rhs.z;
				return *this;
			}

			const Vector& operator+=(const Vector& rhs) {
				x += rhs.x;
				y += rhs.y;
				z += rhs.z;
				return *this;
			}

			const Vector operator*(value_type rhs) const
			{
				return Vector(x * rhs, y * rhs, z * rhs);
			}


			const Vector Cross(const Vector& rhs) const {
				return Vector::Cross(*this, rhs);
			}

			const Vector Normalize() const {
				value_type InnerValue = sqrt(x * x + y * y + z * z);
				if (Math::Equal(InnerValue, 0.0f))
					return Vector(0.f, 0.f, 0.f);
				return Vector(x / InnerValue, y / InnerValue, z / InnerValue);
			}

			const Vector Normalize() {
				value_type InnerValue = sqrt(x * x + y * y + z * z);
				if (Math::Equal(InnerValue, 0.0f))
					return Vector(0.f, 0.f, 0.f);
				return Vector(x / InnerValue, y / InnerValue, z / InnerValue);
			}

			void SelfNormalize() {
				value_type InnerValue = sqrt(x * x + y * y + z * z);
				if (Math::Equal(InnerValue, 0.0f)) {
					x = 0.f;
					y = 0.f;
					z = 0.f;
				}
				this->x /= InnerValue;
				this->y /= InnerValue;
				this->z /= InnerValue;
			}

			const value_type Dot(const Vector& rhs) const {
				return x * rhs.x + y * rhs.y + z * rhs.z;
			}


			static const Vector Cross(const Vector& lhs, const Vector& rhs) {
				// i  j  k
				// x0,y0,z0
				// x1,y1,z1
				return Vector(
					lhs.y * rhs.z - lhs.z * rhs.y,
					lhs.z * rhs.x - lhs.x * rhs.z,
					lhs.x * rhs.y - lhs.y * rhs.x
				);
			}

		public:
			union
			{
				value_type _data[3];
				struct {
					union { value_type x, r, s; };
					union { value_type y, g, t; };
					union { value_type z, b, p; };
				};
			};
		};

	}
}



#endif