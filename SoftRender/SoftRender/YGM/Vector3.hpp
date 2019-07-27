#ifndef _YRENDER_YGM_VECTOR3_HPP_
#define _YRENDER_YGM_VECTOR3_HPP_

#include <YGM/Vector.hpp>

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
				y(static_cast<value_type>(x)),
				z(static_cast<value_type>(x)){}
			explicit Vector(value_type val) : Vector(val, val, val) {}
			Vector() : Vector(static_cast<value_type>(0)){}
			Vector(const Vector& rhs) : Vector(rhs.x,rhs.y,rhs.z){}
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