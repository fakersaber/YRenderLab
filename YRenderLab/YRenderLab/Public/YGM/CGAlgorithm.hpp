#ifndef _YRENDER_YGM_GCALGORITHM_HPP
#define _YRENDER_YGM_GCALGORITHM_HPP



namespace YGM {

	//float3 ACESToneMapping(float3 color, float adapted_lum)
	//{
	//	const float A = 2.51f;
	//	const float B = 0.03f;
	//	const float C = 2.43f;
	//	const float D = 0.59f;
	//	const float E = 0.14f;

	//	color *= adapted_lum;
	//	return (color * (A * color + B)) / (color * (C * color + D) + E);
	//}

	//static void BresenhamDrawLine(int x1, int y1, int x2, int y2)
	//{
	//	int dx = x2 - x1;
	//	int dy = y2 - y1;
	//	int stepx = 1;
	//	int stepy = 1;
	//	if (dx >= 0)
	//	{
	//		stepx = 1;
	//	}
	//	else
	//	{
	//		stepx = -1;
	//		dx = abs(dx);
	//	}
	//	if (dy >= 0)
	//	{
	//		stepy = 1;
	//	}
	//	else
	//	{
	//		stepy = -1;
	//		dy = abs(dy);
	//	}

	//	int deltaX = 2 * dx;
	//	int deltaY = 2 * dy;
	//	if (dx > dy)
	//	{
	//		int p = deltaY - dx; //2¦¤y - ¦¤x
	//		for (int i = 0; i <= dx; ++i)
	//		{
	//			//if (x1 >= 0 && x1 < _RenderDevice->GetWidth() && y1 >= 0 && y1 < _RenderDevice->GetHeight())
	//			_RenderDevice->DrawPixel(x1, y1);
	//			if (p >= 0)
	//			{
	//				p -= deltaX; //pi>0  pi+1 = pi + 2¦¤y - 2¦¤x
	//				y1 += stepy;
	//			}
	//			p += deltaY;
	//			x1 += stepx;
	//		}
	//	}
	//	else
	//	{
	//		int p = deltaX - dy;//2¦¤x - ¦¤y
	//		for (int i = 0; i <= dy; i++)
	//		{
	//			//if (x1 >= 0 && x1 < _RenderDevice->GetWidth() && y1 >= 0 && y1 < _RenderDevice->GetHeight())
	//			_RenderDevice->DrawPixel(x1, y1);
	//			if (p >= 0)
	//			{
	//				p -= deltaY;
	//				x1 += stepx;
	//			}
	//			p += deltaX;
	//			y1 += stepy;
	//		}
	//	}
	//}
}





#endif