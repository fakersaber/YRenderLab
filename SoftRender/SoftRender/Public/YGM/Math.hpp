#ifndef _YRENDER_YGM_MATH_HPP_
#define _YRENDER_YGM_MATH_HPP_

#include <cmath>

namespace YRender {
	namespace YGM {
		namespace Math {
			constexpr float EPSILON = 1e-6f;
			constexpr float PI = 3.14159265358979323f;

			template<typename T>
			constexpr T ToVal(T orig, T val, T bound = static_cast<T>(0.000001)) {
				const auto delta = orig - val;
				if (delta < bound && delta > -bound)
					return val;
				else
					return orig;
			}

			template<typename T>
			constexpr T ToZero(T orig, T bound = static_cast<T>(0.000001)) {
				return ToVal(orig, static_cast<T>(0), bound);
			}

			template<typename T>
			constexpr bool Equal(T lhs, T rhs, T bound = static_cast<T>(EPSILON)) {
				const auto delta = lhs - rhs;
				if (delta > -bound && delta < bound)
					return true;
				return false;
			}
		}
	}
}



#endif