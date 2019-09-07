#include "Camera.h"



namespace YRender {
	const float Camera::ASPECT_WH = 1.f;
	const float Camera::NEAR_PLANE = 0.01f;
	const float Camera::FAR_PLANE = 15.0f;
	const float Camera::YAW = 0.0f;
	const float Camera::PITCH = 0.0f;
	const float Camera::FOV = 50.0f;

	Camera::Camera(
		const Vector3& pos,
		float aspect,
		float nearPlane,
		float farPlane,
		const Vector3& up,
		Project_Type projectionMode
	)
		: 
		position(pos),
		aspect_wh(aspect),
		nearPlane(nearPlane),
		farPlane(farPlane),
		worldup(up),
		project_mode(projectionMode),
		fov(FOV)
	{
		Camera::updateCameraVectors();
	}




	void Camera::updateCameraVectors() {
		// Calculate the new Front vector
		//opengl下旋转顺序是先旋转pitch -> yaw -> roll
		//考虑在上述坐标系下，x轴实际上会受到pitch


		//front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		//front.y = sin(glm::radians(pitch));
		//front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.SelfNormalize();
		//// Also re-calculate the Right and Up vector
		//right = front.Cross(worldUp).Normalize();  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		//up = right.Cross(front).Normalize();
	}
}
