#include <Public/Basic/BSDF_blinnPhong.h>
#include <Public/Basic/Image/Image.h>

namespace YRender {
	const RGBf BSDF_blinnPhong::F(const Vector3& Wi, const Vector3& Wo, const Vector2& texcoord)
	{
		//vec3 albedo = texture(bsdf.albedoTexture, fs_in.TexCoords).xyz;
		//vec3 diffusecolor = lambert_diffuse(albedo);
		//float wh = normalize(wi + wo);
		//float VoH = dot(wh, wo);
		//vec3 specularcolor = (bsdf.gloss + 2.f) / 8.f * dot(wh, fs_in.Normal) * F_Schlick(0.04, VoH);
		//return specularcolor + diffusecolor;
		return GetAlbedo(texcoord) / YGM::Math::PI;
	}

	float BSDF_blinnPhong::PDF(const Vector3 & Wo, const Vector3 & Wi, const Vector2 & texcoord)
	{
		return 0.0f;
	}

	const RGBf BSDF_blinnPhong::GetAlbedo(const Vector2 & texcoord) const
	{
		if (!albedoTexture || !albedoTexture->IsValid())
			return colorFactor;

		return colorFactor * albedoTexture->Sample(texcoord.x, texcoord.y,Image::Mode::BILINEAR).ToRGB();
	}


}