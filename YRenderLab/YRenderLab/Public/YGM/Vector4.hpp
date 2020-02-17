#ifndef _YRENDER_YGM_VECTOR4_HPP_
#define _YRENDER_YGM_VECTOR4_HPP_

#include <Public/YGM/Vector.hpp>


namespace YGM {
	template <typename T>
	class Vector<4, T> {
	private:
		using value_type = T;
	public:
		template<typename U, typename V, typename W, typename H>
		Vector(U x, V y, W z, H w) :
			x(static_cast<value_type>(x)),
			y(static_cast<value_type>(y)),
			z(static_cast<value_type>(z)),
			w(static_cast<value_type>(w))
		{}
		explicit Vector(value_type val) : Vector(val, val, val, val) {}
		Vector() : Vector(static_cast<value_type>(0)) {}
		Vector(const Vector& rhs) : Vector(rhs.x, rhs.y, rhs.z, rhs.w) {}
		bool operator==(const Vector& rhs) {
			return Math::Equal(x, rhs.x) && Math::Equal(y, rhs.y) && Math::Equal(z, rhs.z) && Math::Equal(w, rhs.w);
		}
		bool operator!=(const Vector& rhs) const {
			return !Math::Equal(x, rhs.x) || !Math::Equal(y, rhs.y) || !Math::Equal(z, rhs.z) || !Math::Equal(w, rhs.w);
		}
		Vector& operator=(const Vector& rhs) {
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			w = rhs.w;
			return *this;
		}

		Vector& operator/=(value_type rhs) {
			x /= rhs;
			y /= rhs;
			z /= rhs;
			w /= rhs;
			return *this;
		}
		const value_type Dot(const Vector& rhs) const {
			return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
		}

		const T& operator[](int i) const { return _data[i]; }
		T& operator[](int i) { return _data[i]; }
	public:
		union
		{
			value_type _data[4];
			struct {
				union { value_type x, r, s; };
				union { value_type y, g, t; };
				union { value_type z, b, p; };
				union { value_type w, a, f; };
			};
		};
	};

}



#endif