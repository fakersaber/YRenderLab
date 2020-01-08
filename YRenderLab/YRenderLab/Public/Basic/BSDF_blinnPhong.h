#ifndef _YRENDER_BASIC_BSDF_BLINNPHONG_H_
#define _YRENDER_BASIC_BSDF_BLINNPHONG_H_

#include <Public/Basic/BSDF.h>


	class BSDF_blinnPhong final : public BSDF {
	public:
		BSDF_blinnPhong(const RGBf& colorFactor = RGBf::White, float gloss = 20.f)
			: colorFactor(colorFactor), albedoTexture(nullptr),gloss(gloss)
		{

		}

	protected:
		virtual ~BSDF_blinnPhong() = default;

	public:
		//BSDFÏî
		virtual const RGBf F(const Vector3& Wo, const Vector3& Wi, const Vector2& texcood) override;

		virtual float PDF(const Vector3& Wo, const Vector3& Wi, const Vector2& texcoord) override;

	private:
		const RGBf GetAlbedo(const Vector2& texcoord) const;

	public:
		RGBf colorFactor;
		float gloss;
		std::shared_ptr<Image> albedoTexture;
		std::shared_ptr<Image> normalTexture;
		std::shared_ptr<Image> specularTexture;
	};


#endif