#ifndef _YRENDER_RAY_HITMANAGER_H_
#define _YRENDER_RAY_HITMANAGER_H_

#include <vector>
#include <Public/Ray/HitInfo.h>


	class HitInfoManager {
	public:
		HitInfoManager(const HitInfoManager& rhs) = delete;

		HitInfoManager& operator=(const HitInfoManager& rhs) = delete;

		static HitInfoManager* GetInstance() {
			static HitInfoManager instance;
			return &instance;
		}

	public:
		bool CheckHit(const Ray& ray, float TStart, float TEnd, HitInfo& hitinfo);


	private:
		HitInfoManager() {};
		~HitInfoManager() {
			for (auto ReleasePtr : GeometricList) {
				delete ReleasePtr;
			}
		};

	public:
		std::vector<HitGeometric*> GeometricList;
	};






#endif