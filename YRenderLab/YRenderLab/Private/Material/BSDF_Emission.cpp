#include <Public/Basic/BSDF_Emission.h>
#include <Public/Viewer/Raster.h>

const RGBf BSDF_Emission::F(const Vector3 & Wo, const Vector3 & Wi, const Vector2 & texcood)
{
	return RGBf();
}

float BSDF_Emission::PDF(const Vector3 & Wo, const Vector3 & Wi, const Vector2 & texcoord)
{
	return 0.0f;
}

void BSDF_Emission::SetCurMaterial(const std::shared_ptr<Raster>& Raster){
	Raster->SetMaterial(shared_this<BSDF_Emission>());
}

