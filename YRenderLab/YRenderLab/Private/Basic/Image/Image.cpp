#include <Public/Basic/Image/Image.h>

#define STB_IMAGE_IMPLEMENTATION
#include <Public/Basic/Image/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <Public/Basic/Image/stb_image_write.h>

#include <Public/Basic/StrApi.h>

namespace YRender {

	Image::Image()
		:data(nullptr), width(0), height(0), channel(0) { }

	Image::Image(int width, int height, int channel){
		this->width = width;
		this->height = height;
		this->channel = channel;
		data = new float[width*height*channel]();
	}


	Image::Image(const std::string& path, bool flip)
		: data(nullptr)
	{
		Load(path, flip);
	}

	Image::Image(Image&& img) {
		width = img.width;
		height = img.height;
		channel = img.channel;
		data = img.data;

		img.data = nullptr;
	}


	Image::~Image() {
		Free();
	}


	bool Image::Load(const std::string& path, bool flip) {
		stbi_set_flip_vertically_on_load(flip);
		if (StrAPI::Is_suffix(path, ".hdr")) {
			auto fdata = stbi_loadf(path.c_str(), &width, &height, &channel, 0);
			if (!fdata) {
				return false;
			}
			const int ValNum = width * height * channel;
			data = new float[ValNum];
			for (int i = 0; i < ValNum; i++)
				data[i] = fdata[i];
			stbi_image_free(fdata);
		}
		else {
			auto fdata = stbi_load(path.c_str(), &width, &height, &channel, 0);
			if (!fdata) {
				return false;
			}
			const int ValNum = width * height * channel;
			data = new float[ValNum];
			for (int i = 0; i < ValNum; ++i) {
				data[i] = fdata[i] / 255.f;
			}
			stbi_image_free(fdata);
		}
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

	const RGBAf Image::SampleBilinear(float u, float v) const
	{
		float xf = YGM::Math::Clamp(u, 0.f, 0.999999f) * width;
		float yf = YGM::Math::Clamp(v, 0.f, 0.999999f) * height;
		int x0 = static_cast<int>(xf);
		int y0 = static_cast<int>(yf);
		int x1 = 0;
		int y1 = 0;
		if (x0 == width - 1)
			x1 = width - 1;
		else
			x1 = x0 + 1;
		if (y0 == height - 1)
			y1 = height - 1;
		else
			y1 = y0 + 1;
		float tx = abs(xf - x0);
		float ty = abs(yf - y0);
		RGBAf mixColor = RGBAf::Lerp(
			RGBAf::Lerp(GetPixel(x0, y0), GetPixel(x1, y0), tx), 
			RGBAf::Lerp(GetPixel(x1, y1), GetPixel(x1, y1), tx),
			ty
		);
		return mixColor;
	}


	const RGBAf Image::GetPixel(int x, int y) const {
		RGBAf rgba(0, 0, 0, 1);
		std::memcpy(rgba._data, &data[(y * width + x) * this->channel], sizeof(float) * channel);
		return rgba;
	}

	bool Image::SaveToPNG(const std::string& fileName, bool flip) {
		if (!IsValid())
			return false;
		stbi_flip_vertically_on_write(flip);
		uint8_t* Todata = new uint8_t[width * height * channel];

		for (int i = 0; i < width * height * channel; ++i) {
			Todata[i] = static_cast<uint8_t>(YGM::Math::Clamp(data[i] * 255.f, 0.f, 255.f));
		}
		auto result = stbi_write_png(fileName.c_str(), width, height, channel, Todata, width * channel);
		delete Todata;
		return result;
	}

	bool Image::IsValid() const {
		return width != 0 && height != 0 && channel != 0 && data != nullptr;
	}

	void Image::Free() {
		delete data;
		data = nullptr;
		width = 0;
		height = 0;
		channel = 0;
	}



	void Image::OutPng(const char* fineName, int width, int height, int channel, void* data_ptr) {
		if (width != 0 && height != 0 && channel != 0 && data_ptr != nullptr) {
			stbi_write_png(fineName, width, height, channel, data_ptr, width * channel);
		}
	}
}