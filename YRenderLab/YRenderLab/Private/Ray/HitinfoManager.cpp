#include <Public/Ray/HitinfoManager.h>

	bool HitInfoManager::CheckHit(const Ray& ray, float TStart, float TEnd, HitInfo& hitinfo) {
		float ClosestDistance = TEnd;
		HitInfo TempHitInfo;
		bool RetVal = false;
		for (auto GeometricPtr : GeometricList) {
			if (GeometricPtr->hit(ray, TStart, ClosestDistance, TempHitInfo)) {
				RetVal = true;
				ClosestDistance = TempHitInfo.t;
				hitinfo = TempHitInfo;
			}
		}
		return RetVal;
	}