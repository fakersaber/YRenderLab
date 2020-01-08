#ifndef _YRENDER_BASIC_BSDF_STANDARDPBR_H_
#define _YRENDER_BASIC_BSDF_STANDARDPBR_H_

#include <Public/Basic/BSDF.h>



	class BSDF_StandardPBR final : public BSDF {
	public:
		BSDF_StandardPBR(const RGBf & colorFactor = RGBf(1.f), float roughnessFactor = 1.f, float metallicFactor = 1.f)
			: colorFactor(colorFactor), roughnessFactor(roughnessFactor), metallicFactor(metallicFactor),
			albedoTexture(nullptr), metallicTexture(nullptr), aoTexture(nullptr) { }

	protected:
		virtual ~BSDF_StandardPBR() = default;

	public:
		//BSDFÏî
		virtual const RGBf F(const Vector3& Wo, const Vector3& Wi, const Vector2& texcood) override;

		virtual float PDF(const Vector3& Wo, const Vector3& Wi, const Vector2& texcoord) override;

	private:
		const RGBf GetAlbedo(const Vector2& texcoord) const;

	public:
		RGBf colorFactor;
		float metallicFactor; //0-1
		float roughnessFactor; //0-1
		std::shared_ptr<Image> albedoTexture;
		std::shared_ptr<Image> metallicTexture;
		std::shared_ptr<Image> roughnessTexture;
		std::shared_ptr<Image> aoTexture;
		std::shared_ptr<Image> normalTexture;
		
	};




#endif