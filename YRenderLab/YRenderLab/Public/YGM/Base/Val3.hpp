#ifndef _YRENDER_YGM_BASE_VAL_3_H
#define _YRENDER_YGM_BASE_VAL_3_H


#include <Public/YGM/Base/Val.hpp>



namespace YRender {
	namespace YGM {
		template <typename T>
		class Val<3, T> {
		private:
			using value_type = T;
			static constexpr int valNum = 3;

		public:
			template<typename U, typename V, typename W>
			Val(U x, V y, W z) :
				x(static_cast<T>(x)),
				y(static_cast<T>(y)),
				z(static_cast<T>(z)) {
				assert(!HasNaN());
			}

			//template<bool _Test,
			//	class _Ty = void>
			//	struct enable_if
			//{	// type is undefined for assumed !_Test
			//};

			//template<class _Ty>
			//struct enable_if<true, _Ty>
			//{	// type is _Ty for _Test
			//	using type = _Ty;
			//};

			//template<bool _Test,
			//	class _Ty = void>
			//	using enable_if_t = typename enable_if<_Test, _Ty>::type;

			template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
			Val(U val) : Val(val, val, val) { }

			Val() : Val(0) { }

			template<typename U, int N, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
			Val(U(&arr)[N]){
				std::memcpy(_data, arr, sizeof(U) * N);
			}

			template<typename U, typename V>
			Val(const Val<2, U> & xy, V z) : Val(xy.x, xy.y, z) { }

			template<typename U, typename V>
			Val(U x, const Val<2, U> & yz) : Val(x, yz.x, yz.y) { }

			template<typename U>
			Val(const Val<3, U> & xyz) : Val(xyz.x, xyz.y, xyz.z) { }

			template<typename U>
			Val(const Val<4, U> & val4) : Val(val4.x, val4.y, val4.z) { }

		public:
			const T& operator[](int i) const { assert(i >= 0 && i < valNum); return _data[i]; }
			T& operator[](int i) { assert(i >= 0 && i < valNum); return _data[i]; }

			bool operator==(const Val& rhs) const {
				return Math::Equal(x, rhs.x) && Math::Equal(y, rhs.y) && Math::Equal(z, rhs.z);
			}

			bool operator!=(const Val& rhs) const {
				return !Math::Equal(x, rhs.x) || !Math::Equal(y, rhs.y) || !Math::Equal(z, rhs.z);
			}

			Val& operator=(const Val& rhs) {
				x = rhs.x;
				y = rhs.y;
				z = rhs.z;
				return *this;
			}

			Val& operator/=(value_type rhs) {
				x /= rhs;
				y /= rhs;
				z /= rhs;
				return *this;
			}

			Val operator/(value_type rhs) const {
				return Val(x / rhs, y / rhs, z / rhs);
			}

			Val& operator*=(value_type rhs) {
				x *= rhs;
				y *= rhs;
				z *= rhs;
				return *this;
			}

			Val operator*(value_type rhs) const{
				return Val(x * rhs, y * rhs, z*rhs);
			}

			Val operator*(const Val& rhs) const {
				return Val(x * rhs.x,y * rhs.y,z * rhs.z);
			}

			Val operator+(const Val& rhs) const {
				return Val(x + rhs.x, y + rhs.y, z + rhs.z);
			}

			Val& operator+=(const Val& rhs) {
				this->x += rhs.x;
				this->y += rhs.y;
				this->z += rhs.z;
				return *this;
			}

		public:
			bool HasNaN() const { return std::isnan<double>(x) || std::isnan<double>(y) || std::isnan<double>(z); }

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