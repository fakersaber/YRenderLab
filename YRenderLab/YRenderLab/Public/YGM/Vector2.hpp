#ifndef _YRENDER_YGM_VECTOR2_HPP_
#define _YRENDER_YGM_VECTOR2_HPP_

#include <Public/YGM/Vector.hpp>


	namespace YGM {
		template <typename T>
		class Vector<2, T> {
		private:
			using value_type = T;
		public:
			template<typename U, typename V>
			Vector(U x, V y) :
				x(static_cast<value_type>(x)),
				y(static_cast<value_type>(y)) {}

			explicit Vector(value_type val) : Vector(val, val) {}

			Vector() : Vector(static_cast<value_type>(0)) {}

			Vector(const Vector& rhs) : Vector(rhs.x, rhs.y) {}

			bool operator==(const Vector& rhs) {
				return Math::Equal(x, rhs.x) && Math::Equal(y, rhs.y);
			}
			bool operator!=(const Vector& rhs) const {
				return !Math::Equal(x, rhs.x) || !Math::Equal(y, rhs.y);
			}
			Vector& operator=(const Vector& rhs) {
				x = rhs.x;
				y = rhs.y;
				return *this;
			}

			const T& operator[](int i) const { assert(i >= 0 && i < 2); return _data[i]; }
			T& operator[](int i) { assert(i >= 0 && i < 2); return _data[i]; }

		public:
			const value_type* Data() const { return _data; }
			value_type* Data() { return _data; }
		public:
			union
			{
				value_type _data[2];
				struct {
					union { value_type x, r, s, u; };
					union { value_type y, g, t, v; };
				};
			};
		};

	}



#endif