#include <Public/Basic/Image/Image.h>

#define STB_IMAGE_IMPLEMENTATION
#include <Public/Basic/Image/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <Public/Basic/Image/stb_image_write.h>


namespace YRender {
	Image::Image(const std::string& path, bool flip)
		:data(nullptr) 
	{
		Load(path, flip);
	}

	Image::~Image() {
		delete data;
		//width = height = channel = 0;
	}


	bool Image::Load(const std::string& path, bool flip) {
		stbi_set_flip_vertically_on_load(flip);
		auto fdata = stbi_load(path.c_str(), &width, &height, &channel, 0);
		if (!fdata) {
			//data = nullptr;
			return false;
		}
		const int byteSize = width * height * channel;
		data = new float[byteSize];
		for (int i = 0; i < byteSize; ++i) {
			data[i] = fdata[i];
		}
		stbi_image_free(fdata);
		return true;
	}

}