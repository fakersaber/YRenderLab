#ifndef _YRENDER_YGM_MATH_HPP_
#define _YRENDER_YGM_MATH_HPP_

#include <cmath>

namespace YRender {
	namespace YGM {
		namespace Math {
			constexpr float EPSILON = 1e-6f;
			constexpr float PI = 3.14159265358979323f;

			inline float Radians(const float degree) {
				return (PI / 180.f) * degree;
			}

			inline float Degree(const float Radians) {
				return (180.f / PI) * Radians;
			}

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

			/*static void BresenhamDrawLine(int x1, int x2, int y1, int y2) {
				int dx = x2 - x1;
				int dy = y2 - y1;
				int stepx = 1;
				int stepy = 1;
				if (dx >= 0)
				{
					stepx = 1;
				}
				else
				{
					stepx = -1;
					dx = abs(dx);
				}
				if (dy >= 0)
				{
					stepy = 1;
				}
				else
				{
					stepy = -1;
					dy = abs(dy);
				}

				int deltaX = 2 * dx;
				int deltaY = 2 * dy;
				if (dx > dy)
				{
					int error = deltaY - dx;
					for (int i = 0; i <= dx; ++i)
					{
						if (x1 >= 0 && x1 < _RenderDevice->GetWidth() && y1 >= 0 && y1 < _RenderDevice->GetHeight())
							_RenderDevice->DrawPixel(x1, y1);
						if (error >= 0)
						{
							error -= deltaX;
							y1 += stepy;
						}
						error += deltaY;
						x1 += stepx;
					}
				}
				else
				{
					int error = deltaX - dy;
					for (int i = 0; i <= dy; i++)
					{
						if (x1 >= 0 && x1 < _RenderDevice->GetWidth() && y1 >= 0 && y1 < _RenderDevice->GetHeight())
							_RenderDevice->DrawPixel(x1, y1);
						if (error >= 0)
						{
							error -= deltaY;
							x1 += stepx;
						}
						error += deltaX;
						y1 += stepy;
					}
				}
			}*/
		}
	}
}



#endif