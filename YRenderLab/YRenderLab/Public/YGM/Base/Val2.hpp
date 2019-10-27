#ifndef _YRENDER_YGM_BASE_VAL_2_H
#define _YRENDER_YGM_BASE_VAL_2_H


#include <Public/YGM/Base/Val.hpp>


namespace YRender {
	namespace YGM {
		template <typename T>
		class Val<2, T> {
		private:
			using value_type = T;
			static constexpr int valNum = 2;

		public:
			template<typename U, int N, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
			Val(U(&arr)[N]) {
				std::memcpy(_data, arr, sizeof(U) * N);
			}

			template<typename U, typename V>
			Val(U x, V y) :
				x(static_cast<value_type>(x)),
				y(static_cast<value_type>(y)) {}

			explicit Val(value_type val) : Val(val, val) {}

			Val() : Val(static_cast<value_type>(0)) {}

			Val(const Val& rhs) : Val(rhs.x, rhs.y) {}


		public:
			const T& operator[](int i) const { assert(i >= 0 && i < valNum); return _data[i]; }
			T& operator[](int i) { assert(i >= 0 && i < valNum); return _data[i]; }

			bool operator==(const Val& rhs) const {
				return Math::Equal(x, rhs.x) && Math::Equal(y, rhs.y);
			}

			bool operator!=(const Val& rhs) const {
				return !Math::Equal(x, rhs.x) || !Math::Equal(y, rhs.y);
			}

			Val& operator=(const Val& rhs) {
				x = rhs.x;
				y = rhs.y;
				return *this;
			}

			Val& operator/=(value_type rhs) {
				x /= rhs;
				y /= rhs;
				return *this;
			}

			Val& operator*=(value_type rhs) {
				x *= rhs;
				y *= rhs;
				return *this;
			}

			Val operator*(value_type rhs) const {
				return Val(x * rhs, y * rhs);
			}

			Val operator+(const Val& rhs) const {
				return Val(x + rhs.x, y + rhs.y);
			}


		public:
			union
			{
				value_type _data[2];
				struct {
					union { value_type x, r, s; };
					union { value_type y, g, t; };
				};
			};
		};
	}
}

#endif