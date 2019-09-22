#ifndef _YRENDER_YGM_BASE_VAL_2_H
#define _YRENDER_YGM_BASE_VAL_2_H


#include <Public/YGM/Base/Val.hpp>


namespace YRender {
	namespace YGM {
		template <typename T>
		class Val<2, T> {


		public:
			template<typename U, int N, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
			Val(U(&arr)[N]) {
				std::memcpy(_data, arr, sizeof(U) * N);
			}
		};
	}
}

#endif