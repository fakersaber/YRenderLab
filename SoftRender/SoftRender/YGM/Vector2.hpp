#ifndef _YRENDER_YGM_VECTOR2_HPP_
#define _YRENDER_YGM_VECTOR2_HPP_

#include <YGM/Vector.hpp>

namespace YRender {
	namespace YGM {
		template<typename T>
		class Vector<2, T> {
		private:
			using value_type = T;

			template <typename U,typename V>
			Vector(U x, V y) :
				x(static_cast<value_type>(0)),
				y(static_cast<value_type>(0)) {}

		};
	}
}

#endif
