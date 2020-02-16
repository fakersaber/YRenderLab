#ifndef _YRENDER_YGM_BASE_BASIC_VAL_3_H
#define _YRENDER_YGM_BASE_BASIC_VAL_3_H

#include <Public/YGM/Base/Val3.hpp>
#include <Public/YGM/Base/Basic_Val.hpp>
//
//#include <CppUtil/Basic/Math.h>

#include <vector>


namespace YGM {
	template <typename T, typename ImplT>
	class Basic_Val<3, T, ImplT> : public Val<3, T> {
	public:
		using Val<3, T>::Val;



		//static const ImplT Lerp(const ImplT& s0, const ImplT& s1, T t) {
		//	//return 
		//}
	};
}


#endif 