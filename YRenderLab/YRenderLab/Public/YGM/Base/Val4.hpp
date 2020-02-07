#ifndef _YRENDER_YGM_BASE_VAL_4_H
#define _YRENDER_YGM_BASE_VAL_4_H


#include <Public/YGM/Base/Val.hpp>


namespace YGM {
	template <typename T>
	class Val<4, T> {
	private:
		using value_type = T;
		static constexpr int valNum = 4;
	public:
		template<typename U, typename V, typename W, typename H>
		Val(U x, V y, W z, H w) :
			x(static_cast<T>(x)),
			y(static_cast<T>(y)),
			z(static_cast<T>(z)),
			w(static_cast<T>(w))
		{
			assert(!HasNaN());
		}

		template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
		Val(U val) : Val(val, val, val, val) {}

		Val() : Val(0) {}

		template<typename U, int N, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
		Val(U(&arr)[N]) {
			std::memcpy(_data, arr, sizeof(U) * N);
		}
		template<typename U, typename V, typename W>
		Val(const Val<2, U> & xy, V z, W w) : Val(xy.x, xy.y, z, w) { }

		template<typename U, typename V, typename W>
		Val(U x, const Val<2, V> & yz, W w) : Val(x, yz.x, yz.y, w) { }

		template<typename U, typename V, typename W>
		Val(U x, V y, const Val<2, W> & zw) : Val(x, y, zw.x, zw.y) { }

		template<typename U, typename V>
		Val(const Val<2, U> & xy, const Val<2, V> & zw) : Val(xy.x, xy.y, zw.x, zw.y) { }

		template<typename U, typename V>
		Val(const Val<3, U> & xyz, V w) : Val(xyz.x, xyz.y, xyz.z, w) { }

		template<typename U, typename V>
		Val(U x, const Val<3, V> & yzw) : Val(x, yzw.x, yzw.y, yzw.z) { }

		template<typename U>
		explicit Val(const Val<4, U> & val4) : Val(val4.x, val4.y, val4.z, val4.w) { }

	public:
		const T& operator[](int i) const { assert(i >= 0 && i < valNum); return _data[i]; }
		T& operator[](int i) { assert(i >= 0 && i < valNum); return _data[i]; }

		bool operator==(const Val & rhs) const {
			return Math::Equal(x, rhs.x) && Math::Equal(y, rhs.y) && Math::Equal(z, rhs.z) && Math::Equal(w, rhs.w);
		}

		bool operator!=(const Val & rhs) const {
			return !Math::Equal(x, rhs.x) || !Math::Equal(y, rhs.y) || !Math::Equal(z, rhs.z) || !Math::Equal(w, rhs.w);
		}

		Val& operator=(const Val & rhs) {
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			w = rhs.w;
			return *this;
		}

	public:
		bool HasNaN() const { return std::isnan<double>(x) || std::isnan<double>(y) || std::isnan<double>(z) || std::isnan<double>(w); }

	public:
		union
		{
			value_type _data[4];
			struct {
				union { value_type x, r, s; };
				union { value_type y, g, t; };
				union { value_type z, b, p; };
				union { value_type w, a, q; };
			};
		};
	};
}


#endif