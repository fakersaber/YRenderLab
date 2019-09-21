#ifndef _YRENDER_YGM_RGBA_H_
#define _YRENDER_YGM_RGBA_H_

namespace YRender {
	namespace YGM {
		template <typename T>
		class RGBA {

		public:
			explicit RGBA(T val) : RGBA(val, val, val, 1) { }

			RGBA() : RGBA(static_cast<T>(0)) { }

			template<typename U, typename V>
			RGBA(const Val<2, U>& rg, V b) : RGBA(rg, b, 1) { }

			template<typename U, typename V>
			RGBA(U r, const Val<2, V> & gb) : RGBA(r, gb, 1) { }

			template<typename U>
			RGBA(const Val<3, U> & rbg) : RGBA(rbg, 1) { }

			template<typename U, typename V>
			RGBA(const RGB<U> & rgb, V a) : RGBA(rgb.r, rgb.g, rgb.b, a) { }

			template<typename U, typename V>
			RGBA(const RGB<U> & rgb) : RGBA(rgb.r, rgb.g, rgb.b, 1) { }

			template<typename U>
			RGBA(const RGBA<U>& rgba) : RGBA(rgba.r, rgba.g, rgba.b, rgba.a) { }






		public:
			union
			{
				Type _data[4];
				struct {
					union { Type x, r, s; };
					union { Type y, g, t; };
					union { Type z, b, p; };
					union { Type w, a, f; };
				};
			};
		};
	}
}





#endif