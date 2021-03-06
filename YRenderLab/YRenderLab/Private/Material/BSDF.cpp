#include <Public/Basic/BSDF.h>
#include <Public/YGM/Matrix3.hpp>
#include <Public/Viewer/Raster.h>

void BSDF::SetCurMaterial(const std::shared_ptr<Raster>& Raster){

}

//注意是tangent cross nromal，因为假设原模型法线坐标系为左手系，z为nromal，up为bitangent
Vector3 BSDF::TangentSpaceToWorld(const Vector3& worldTangent, const Vector3& worldNormal, const Vector3& TangentSpaceNormal)
{
	const Vector3 bitangent = worldTangent.Cross(worldNormal);
	const Mat3f TangentToWorld{
		Vector3(worldTangent.x,bitangent.x,worldNormal.x),
		Vector3(worldTangent.y,bitangent.y,worldNormal.y),
		Vector3(worldTangent.z,bitangent.z,worldNormal.z)
	};
	return TangentToWorld * TangentSpaceNormal;
}