#ifndef _YRENDER_BASIC_IMAGE_IMAGE_H
#define _YRENDER_BASIC_IMAGE_IMAGE_H

#include <string>
#include <Public/YGM/RGBA.hpp>

namespace YRender {

	//opengl是从左下，但stb图片是左上
	class Image {
	public:
		Image();
		Image(const std::string& path, bool flip = false);
		explicit Image(Image&& image);
		explicit Image(const Image& image);
		Image& operator=(const Image& image);
		Image& operator=(Image&& image);
		virtual ~Image();

	private:
		float At(int x, int y, int channel) const;
		void Free() const;


	public:
		bool Load(const std::string& path, bool flip = false);
		const RGBAf SampleNearest(float u, float v) const;
		const RGBAf GetPixel(int x, int y) const;


	private:
		float* data;
		int width;
		int height;
		int channel;
		//std::string path;
	};
}


#endif