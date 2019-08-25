#include <Public/YGM/Transform.h>

namespace YRender {
	namespace YGM {



		const Transform Transform::LookAt(const Vector3& pos, const Vector3& target, const Vector3& up) {
			//[right.x, camera_up.x, -front.x, -dot(pos,right)]
			//[right.y, camera_up.y, -front.y, -dot(pos,camera_up)]
			//[right.z, camera_up.z, -front.z, dot(pos,front)]
			//[0,0,0,1]

			const auto front = (target - pos).Normalize();
			const Vector3 right = front.Cross(up).Normalize();
			const Vector3 camera_up = right.Cross(front);

			Mat4f WorldToCamera;
			WorldToCamera(0, 0) = right.x;
			WorldToCamera(1, 0) = right.y;
			WorldToCamera(2, 0) = right.z;
			WorldToCamera(0, 1) = camera_up.x;
			WorldToCamera(1, 1) = camera_up.y;
			WorldToCamera(2, 1) = camera_up.z;
			WorldToCamera(0, 2) = -front.x;
			WorldToCamera(1, 2) = -front.y;
			WorldToCamera(2, 2) = -front.z;
			WorldToCamera(0, 3) = -right.Dot(pos);
			WorldToCamera(1, 3) = -camera_up.Dot(pos);
			WorldToCamera(2, 3) = front.Dot(pos);

			


			return Transform();
		}
	}
}