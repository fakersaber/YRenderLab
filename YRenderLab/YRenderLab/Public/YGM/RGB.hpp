#ifndef _YRENDER_YGM_RGB_H_
#define _YRENDER_YGM_RGB_H_

#include <Public/YGM/Base/Basic_Val3.hpp>


namespace YRender {
	namespace YGM {
		template <typename T>
		class RGB : public Basic_Val<3, T, RGB<T>> {
		public:
			using Basic_Val<3, T, RGB<T>>::Basic_Val;

		public:
			explicit RGB(T val) : RGB(val, val, val) { }

			RGB() : RGB(static_cast<T>(0)) { }

			template<typename U, typename V>
			RGB(const Val<2, U>& rg, V b) : RGB(rg, b) { }

			template<typename U, typename V>
			RGB(U r, const Val<2, V>& gb) : RGB(r, gb) { }


			

		public:
			RGB& operator=(const Vector<3,T>& rhs) {
				this->x = rhs.x;
				this->y = rhs.y;
				this->z = rhs.z;
				return *this;
			}

			RGB& operator+=(const Vector<3, T>& rhs) {
				this->x += rhs.x;
				this->y += rhs.y;
				this->z += rhs.z;
				return *this;
			}

			RGB& operator+=(const RGB& rhs) {
				this->x += rhs.x;
				this->y += rhs.y;
				this->z += rhs.z;
				return *this;
			}

		public:
			static RGB<T> Red;
			static RGB<T> White;
		};

	}
	template <typename T>
	using RGB = YGM::RGB<T>;

	using RGBf = RGB<float>;

	template <typename T>
	YGM::RGB<T> YGM::RGB<T>::Red = RGB<T>(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0));

	template <typename T>
	YGM::RGB<T> YGM::RGB<T>::White = RGB<T>(static_cast<T>(1), static_cast<T>(1), static_cast<T>(1));

}


#endif