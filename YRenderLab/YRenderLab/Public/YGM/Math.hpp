#ifndef _YRENDER_YGM_MATH_HPP_
#define _YRENDER_YGM_MATH_HPP_

#include <cmath>

namespace YRender {
	namespace YGM {
		namespace Math {
			//smallest such that 1.0f + FLT_EPSILON !=1.0f
			static constexpr float EPSILON = 1.192092896e-07f;
			static constexpr float PI = 3.14159265358979323f;
			static constexpr float MaxFloat = 3.402823466e+38F;

			inline float Radians(const float degree) {
				return (PI / 180.f) * degree;
			}

			inline float Degree(const float Radians) {
				return (180.f / PI) * Radians;
			}

			template<typename T, typename U>
			inline T Lerp(T v0, T v1, U t) {
				return (static_cast<U>(1) - t)*v0 + t * v1;
			}

			template<typename T>
			constexpr T ToVal(T orig, T val, T bound = static_cast<T>(EPSILON)) {
				const auto delta = orig - val;
				if (delta < bound && delta > -bound)
					return val;
				else
					return orig;
			}

			template<typename T>
			constexpr T ToZero(T orig, T bound = static_cast<T>(EPSILON)) {
				return ToVal(orig, static_cast<T>(0), bound);
			}

			template<typename T>
			constexpr bool Equal(T lhs, T rhs, T bound = static_cast<T>(EPSILON)) {
				const auto delta = lhs - rhs;
				if (delta > -bound && delta < bound)
					return true;
				return false;
			}

			template<typename T>
			constexpr T Max(T lhs, T rhs) {
				if (lhs >= rhs)
					return lhs;
				return rhs;
			}

			template<typename T>
			constexpr T Min(T lhs,T rhs) {
				if (lhs <= rhs)
					return lhs;
				return rhs;
			}


			template<typename T>
			T Clamp(T value, T minV, T maxV) {
				if (value < minV)
					return minV;
				else if (value > maxV)
					return maxV;
				else
					return value;
			}



		}
	}
}



#endif