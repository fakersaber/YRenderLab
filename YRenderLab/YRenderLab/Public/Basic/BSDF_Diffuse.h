#ifndef _YRENDER_BASIC_BSDF_DIFFUSE_H_
#define _YRENDER_BASIC_BSDF_DIFFUSE_H_

#include <Public/Basic/BSDF.h>

namespace YRender{
	class BSDF_Diffuse final : public BSDF {
	public:
		BSDF_Diffuse(const RGBf& colorFactor = RGBf::White)
			: colorFactor(colorFactor), albedoTexture(nullptr){
		}

	protected:
		virtual ~BSDF_Diffuse() = default;

	public:
		//BSDFÏî
		virtual const RGBf F(const Vector3& Wo, const Vector3& Wi, const Vector2& texcood) override;

		virtual float PDF(const Vector3& Wo, const Vector3& Wi, const Vector2& texcoord) override;

	private:
		const RGBf GetAlbedo(const Vector2& texcoord) const;

	public:
		RGBf colorFactor;
		std::shared_ptr<Image> albedoTexture;
	};
}

#endif