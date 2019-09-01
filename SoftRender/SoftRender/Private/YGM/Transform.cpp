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
			// [ R^T -RT ]
			// [ 0   1 ]
			//
			// cameraToWorld
			// [ R   T ]
			// [ 0   1 ]
			Vector3 front = (target - pos).Normalize();
			Vector3 right = front.Cross(up).Normalize();
			Vector3 camera_up = right.Cross(front);

			Mat4f WorldToCamera;
			WorldToCamera(0, 0) = right.x;
			WorldToCamera(0, 1) = camera_up.x;
			WorldToCamera(0, 2) = -front.x;
			WorldToCamera(1, 0) = right.y;
			WorldToCamera(1, 1) = camera_up.y;
			WorldToCamera(1, 2) = -front.y;
			WorldToCamera(2, 0) = right.z;
			WorldToCamera(2, 1) = camera_up.z;
			WorldToCamera(2, 2) = -front.z;
			WorldToCamera(0, 3) = -right.Dot(pos);
			WorldToCamera(1, 3) = -camera_up.Dot(pos);
			WorldToCamera(2, 3) = front.Dot(pos);


			Mat4f CameraToWorld;
			CameraToWorld(0, 0) = right.x;
			CameraToWorld(0, 1) = right.y;
			CameraToWorld(0, 2) = right.z;
			CameraToWorld(1, 0) = camera_up.x;
			CameraToWorld(1, 1) = camera_up.y;
			CameraToWorld(1, 2) = camera_up.z;
			CameraToWorld(2, 0) = -front.x;
			CameraToWorld(2, 1) = -front.y;
			CameraToWorld(2, 2) = -front.z;
			CameraToWorld(0, 3) = pos.x;
			CameraToWorld(1, 3) = pos.y;
			CameraToWorld(2, 3) = pos.z;

			return Transform(WorldToCamera, CameraToWorld);
		}

		const Transform Transform::Perspective(const float fovy, const float aspect, const float zNear, const float zFar) {

			 
		}
	}
}