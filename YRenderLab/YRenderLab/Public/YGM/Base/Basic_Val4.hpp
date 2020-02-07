#ifndef _YRENDER_YGM_BASE_BASIC_VAL_4_H
#define _YRENDER_YGM_BASE_BASIC_VAL_4_H

#include <Public/YGM/Base/Val4.hpp>
#include <Public/YGM/Base/Basic_Val.hpp>
//
//#include <CppUtil/Basic/Math.h>

#include <vector>

namespace YGM {
	template <typename T, typename ImplT>
	class Basic_Val<4, T, ImplT> : public Val<4, T> {
	public:
		using Val<4, T>::Val;


	public:
		static const ImplT Lerp(const ImplT& s0, const ImplT& s1, T t) {
			return ImplT(
				Math::Lerp(s0.x, s1.x, t),
				Math::Lerp(s0.y, s1.y, t),
				Math::Lerp(s0.z, s1.z, t),
				Math::Lerp(s0.w, s1.w, t)
			);
		}
	};
}


#endif 