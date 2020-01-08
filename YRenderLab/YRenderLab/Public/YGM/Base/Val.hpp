#ifndef _YRENDER_YGM_BASE_VAL_H
#define _YRENDER_YGM_BASE_VAL_H

#include <Public/YGM/Math.hpp>
#include <cassert>
#include <iostream>


	namespace YGM {
		template <unsigned N,typename T>
		class Val;
	}

	template <int N, typename T>
	using Val = YGM::Val<N, T>;

	using Val1f = Val<1, float>;
	using Val1i = Val<1, int>;
	using Val1 = Val1f;

	using Val2f = Val<2, float>;
	using Val2i = Val<2, int>;
	using Val2 = Val2f;

	using Val3f = Val<3, float>;
	using Val3i = Val<3, int>;
	using Val3 = Val3f;

	using Val4f = Val<4, float>;
	using Val4i = Val<4, int>;
	using Val4 = Val4f;



#endif