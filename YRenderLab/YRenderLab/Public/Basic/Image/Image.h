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
		enum class Mode {
			NEAREST,
			BILINEAR,
		};
	public:
		Image();
		Image(int width, int height, int channel);
		Image(const std::string& path, bool flip = false);
		Image(Image&& image);
		Image(const Image& image);
		Image& operator=(const Image& image);
		Image& operator=(Image&& image);
		virtual ~Image();

	private:
		//float At(int x, int y, int channel) const;
		void Free();

	public:
		bool IsValid() const;
		bool Load(const std::string& path, bool flip = false);
		const RGBAf SampleNearest(float u, float v) const;
		const RGBAf SampleBilinear(float u, float v) const;
		const RGBAf GetPixel(int x, int y) const;
		const int GetChannel() const { return channel; }
		const int GetWidth() const { return width; }
		const int GetHeight() const { return height; }
		float* GetData() const { return data; }
		bool SaveToPNG(const std::string& fileName, bool flip = false);
		const RGBAf Sample(float u, float v, Mode mode) const {
			switch (mode)
			{
			case Mode::NEAREST:
				return SampleNearest(u, v);
				break;
			case Mode::BILINEAR:
				return SampleBilinear(u, v);
				break;
			default:
				return RGBAf(0.f);
			}
		}

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