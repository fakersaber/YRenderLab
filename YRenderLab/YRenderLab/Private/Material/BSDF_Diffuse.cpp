#include <Public/Basic/BSDF_Diffuse.h>
#include <Public/Basic/Image/Image.h>

namespace YRender {
	const RGBf BSDF_Diffuse::F(const Vector3& Wo, const Vector3& Wi, const Vector2& texcoord)
	{
		return GetAlbedo(texcoord) / YGM::Math::PI;
	}

	float BSDF_Diffuse::PDF(const Vector3 & Wo, const Vector3 & Wi, const Vector2 & texcoord)
	{
		return 0.0f;
	}

	const RGBf BSDF_Diffuse::GetAlbedo(const Vector2 & texcoord) const
	{
		if (!albedoTexture || !albedoTexture->IsValid())
			return colorFactor;

		return colorFactor * albedoTexture->Sample(texcoord.x, texcoord.y,Image::Mode::BILINEAR).ToRGB();
	}
}