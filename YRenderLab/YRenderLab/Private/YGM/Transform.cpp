#include <Public/YGM/Transform.h>

namespace YGM {
	const Transform Transform::LookAt(const Vector3& pos, const Vector3& target, const Vector3& up) {
		//Vec3 right = front.Cross(up);
		//if (right.IsZero()) {
		//	Vec3 newUp = up.Normalize();
		//	newUp.MinComponent() = 1;
		//	right = front.Cross(newUp).Normalize();
		//}
		//const Vec3 camUp = right.Cross(front);
		Vector3 front = (target - pos).Normalize();
		const Vector3 right = front.Cross(up).Normalize();
		Vector3 camera_up = right.Cross(front);
		Mat4f worldToCamera;
		worldToCamera(0, 0) = right.x;
		worldToCamera(0, 1) = right.y;
		worldToCamera(0, 2) = right.z;

		worldToCamera(1, 0) = camera_up.x;
		worldToCamera(1, 1) = camera_up.y;
		worldToCamera(1, 2) = camera_up.z;

		worldToCamera(2, 0) = -front.x;
		worldToCamera(2, 1) = -front.y;
		worldToCamera(2, 2) = -front.z;

		worldToCamera(0, 3) = -right.Dot(pos);
		worldToCamera(1, 3) = -camera_up.Dot(pos);
		worldToCamera(2, 3) = front.Dot(pos);

		Mat4f CameraToWorld;
		CameraToWorld(0, 0) = right.x;
		CameraToWorld(1, 0) = right.y;
		CameraToWorld(2, 0) = right.z;

		CameraToWorld(0, 1) = camera_up.x;
		CameraToWorld(1, 1) = camera_up.y;
		CameraToWorld(2, 1) = camera_up.z;

		CameraToWorld(0, 2) = -front.x;
		CameraToWorld(1, 2) = -front.y;
		CameraToWorld(2, 2) = -front.z;

		CameraToWorld(0, 3) = pos.x;
		CameraToWorld(1, 3) = pos.y;
		CameraToWorld(2, 3) = pos.z;

		return Transform(worldToCamera, CameraToWorld);
	}

	const Mat4f Transform::Perspective(const float fovy, const float aspect, const float zNear, const float zFar) {
		const float tanHalfFovy = tan(YGM::Math::Radians(fovy * 0.5f));
		Mat4f PerspectiveMat(0.f);
		PerspectiveMat(0, 0) = 1.f / (tanHalfFovy * aspect);
		PerspectiveMat(1, 1) = 1.f / tanHalfFovy;
		PerspectiveMat(2, 2) = -(zFar + zNear) / (zFar - zNear);
		PerspectiveMat(2, 3) = -(2.f * zNear * zFar) / (zFar - zNear);
		PerspectiveMat(3, 2) = -1.f;
		return PerspectiveMat;
	}
}