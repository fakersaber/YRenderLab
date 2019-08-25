#include <Public/YGM/Transform.h>

namespace YRender {
	namespace YGM {



		const Transform Transform::LookAt(const Vector3& pos, const Vector3& target, const Vector3& up) {

			// R
			// [ right  ]
			// [ camUp  ]
			// [ -front ]
			//
			// T
			// [ pos.x ]
			// [ pos.y ]
			// [ pos.z ]
			//
			// worldToCamera
			// [ R -RT ]
			// [ 0   1 ]
			//
			// cameraToWorld
			// [ R^T  T ]
			// [ 0    1 ]

			//[right.x, right.y, right.z, -right.dot(pos)]
			//[camera_up.x, camera_up.y, camera.up.z, -camera_up.dot(pos)]
			//[-front.x, -front.y, -front.z, front.dot(pos)]
			//[0,0,0,1]

			const auto front = (target - pos).Normalize();
			Vector3 right = front.Cross(up).Normalize();
			const Vector3 camera_up = right.Cross(front);

			Mat4f WorldToCamera;





			return Transform();
		}
	}
}