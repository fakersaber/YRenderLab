#ifndef _YRENDER_BASIC_IMAGE_IMAGE_H
#define _YRENDER_BASIC_IMAGE_IMAGE_H

#include <string>
#include <Public/YGM/RGBA.hpp>
#include <Public/Basic/YHeapObject.h>

namespace YRender {

	//opengl是从左下，但stb图片是左上

	//Hdr = float
	//rgb = unsigned char
	class Image : public YHeapObject {
	public:
		Image();
		Image(const std::string& path, bool flip = false);
		explicit Image(Image&& image);
		explicit Image(const Image& image);
		Image& operator=(const Image& image);
		Image& operator=(Image&& image);
		virtual ~Image();

	private:
		//float At(int x, int y, int channel) const;
		void Free();
		bool IsValid() const;

	public:
		bool Load(const std::string& path, bool flip = false);
		const RGBAf SampleNearest(float u, float v) const;
		const RGBAf GetPixel(int x, int y) const;
		bool SaveToPNG(const std::string& fileName, bool flip = false);


	public:
		static void OutPng(const char* fineName, int width, int height, int channel, void* data_ptr);


	private:
		float* data;
		int width;
		int height;
		int channel;
		//std::string path;
	};
}


#endif