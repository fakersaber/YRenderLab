#ifndef _YRENDER_YGM_RGBA_H_
#define _YRENDER_YGM_RGBA_H_

#include <Public/YGM/Base/Basic_Val4.hpp>

#include <Public/YGM/RGB.hpp>


	namespace YGM {
		template <typename T>
		class RGBA : public Basic_Val<4, T, RGBA<T>> {

		public:
			using Basic_Val<4, T, RGBA<T>>::Basic_Val;

		//public:
		//	explicit RGBA(T val) : RGBA(val, val, val, 1) { }

		//	RGBA() : RGBA(static_cast<T>(0)) { }

		//	template<typename U, typename V>
		//	RGBA(const Val<2, U>& rg, V b) : RGBA(rg, b, 1) { }

		//	template<typename U, typename V>
		//	RGBA(U r, const Val<2, V> & gb) : RGBA(r, gb, 1) { }

		//	template<typename U>
		//	RGBA(const Val<3, U>& rbg) : RGBA(rbg, 1) { }


		public:
			const uint32_t RBGA2UINT() const
			{
				//gdi高位为红色
				uint32_t result_value = 0ul;
				uint8_t* ptr = reinterpret_cast<uint8_t*>(&result_value);
				*ptr = static_cast<uint8_t>(this->b * this->a * 255.f);
				ptr += 1;
				*ptr = static_cast<uint8_t>(this->g * this->a * 255.f);
				ptr += 1;
				*ptr = static_cast<uint8_t>(this->r * this->a * 255.f);
				return result_value;
			}

			RGB<T> ToRGB() const {
				return RGB<T>(this->r, this->g, this->b) * this->a;
			}

		public:
			static RGBA<T> Red;
			static RGBA<T> White;
		};
	}
	template <typename T>
	using RGBA = YGM::RGBA<T>;
	using RGBAf = RGBA<float>;

	template <typename T>
	YGM::RGBA<T> YGM::RGBA<T>::Red = RGBA<T>(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),static_cast<T>(1));

	template <typename T>
	YGM::RGBA<T> YGM::RGBA<T>::White = RGBA<T>(static_cast<T>(1), static_cast<T>(1), static_cast<T>(1), static_cast<T>(1));


#endif