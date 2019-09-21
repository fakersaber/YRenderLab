#ifndef _YRENDER_BASIC_IMAGE_IMAGE_H
#define _YRENDER_BASIC_IMAGE_IMAGE_H

#include <string>

namespace YRender {
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
		bool Load(const std::string& path, bool flip = false);

	public:




	private:
		float* data;
		int width;
		int height;
		int channel;
		//std::string path;
	};
}


#endif