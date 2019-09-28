#include <Public/Basic/Image/Image.h>

#define STB_IMAGE_IMPLEMENTATION
#include <Public/Basic/Image/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <Public/Basic/Image/stb_image_write.h>


namespace YRender {

	Image::Image()
		:data(nullptr), width(0), height(0), channel(0) { }


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
			data[i] = fdata[i] / 255.f;
		}
		stbi_image_free(fdata);
		return true;
	}


	const RGBAf Image::SampleNearest(float u, float v) const {
		u = YGM::Math::Clamp(u, 0.f, 0.999999f);
		v = YGM::Math::Clamp(v, 0.f, 0.999999f);
		float xf = u * width;
		float yf = v * height;
		int xi = static_cast<int>(xf);
		int yi = static_cast<int>(yf);
		return GetPixel(xi, yi);
	}

	const RGBAf Image::GetPixel(int x, int y) const {
		RGBAf rgba(0, 0, 0, 1);
		std::memcpy(rgba._data, &data[(y * width + x) * this->channel], sizeof(float) * channel);
		//for (int i = 0; i < channel; i++)
		//	rgba[i] = At(x, y, i);
		return rgba;
	}

	float Image::At(int x, int y, int index) const {
		//assert(index < this->channel);
		//return data[(y*width + x) * this->channel + index];
	}


	void Image::Free() const {
		delete data;
		int width = 0;
		int height = 0;
		int channel = 0;
	}

}