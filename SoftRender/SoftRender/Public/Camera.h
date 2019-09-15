#ifndef _YRENDER_CAMERA_H
#define _YRENDER_CAMERA_H

#include <Public/YGM/Transform.h>

namespace YRender {
	class Camera {
	public:
		enum Project_Type {
			Orthogonal = 1,
			Perspective = 2
		};

	public:
		Camera(
			const Vector3& pos = Vector3(0.f, -4.f, -4.f),
			float yaw = Camera::YAW,
			float pitch = Camera::PITCH,
			float aspect = Camera::ASPECT_WH,
			float nearPlane = Camera::NEAR_PLANE,
			float farPlane = Camera::FAR_PLANE,
			const Vector3& up = Vector3(0.f, 1.f, 0.f),
			Project_Type projectionMode = Project_Type::Perspective
		);

	public:
		Mat4f GetViewMatrix() const { return YGM::Transform::LookAt(position, position + front, up).GetMatrix(); }
		Mat4f GetProjectMatrix() const { return YGM::Transform::Perspective(fov, aspect_wh, nearPlane, farPlane).GetMatrix(); }
		void SetCameraPose(const Vector3& pos, float yaw, float pitch);
		Project_Type GetCameraMode() { return project_mode; }
		const Vector3& GetCameraPos() const { return position; }

	private:
		void updateCameraVectors();

	private:
		Vector3 position;
		Vector3 front;
		Vector3 up;
		Vector3 right;
		Vector3 worldup;
		float yaw;
		float pitch;
		float fov;
		float aspect_wh;
		float nearPlane;
		float farPlane;
		Project_Type project_mode;

		// Default camera values
		static const float ASPECT_WH;
		static const float NEAR_PLANE;
		static const float FAR_PLANE;
		static const float YAW;
		static const float PITCH;
		static const float FOV;
	};
}

#endif