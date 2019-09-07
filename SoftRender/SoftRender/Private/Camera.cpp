#include <Public/Camera.h>



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
		//在此注意camera的front是根据Eular角更新的，而LookAt的target实际上就是pos + front
		//						  [cos(yaw),  sin(yaw)sin(pitch),  sin(yaw)cos(pitch)]
		// R(yaw) * R(pitch) == > [0,         cos(pitch),          -sin(pitch)]
		//					      [-sin(yaw), cos(yaw)sin(pitch),  cos(yaw)cos(pitch)]
		//接着表示front向量
		//								  [-sin(yaw)cos(pitch)]
		// R(yaw) * R(pitch) * front == > [sin(pitch)]
		//								  [-cos(yaw)cos(pitch)]
		//上述表示方案是yaw和pitch起始值从0开始的推导

		//u老师的运算方式与opengl运算代码相同,yaw的起始值为-PI/2,实际变为了yaw - PI/2,
		//								 [-sin(yaw), -cos(yaw)sin(pitch),  -cos(yaw)cos(pitch)]
		// R(yaw - PI/2) * R(pitch) == > [0,         cos(pitch),          -sin(pitch)]
		//								 [cos(yaw), -sin(yaw)sin(pitch),  -sin(yaw)cos(pitch)]
		//接着同理表示front向量
		//								  [cos(yaw)cos(pitch)]
		// R(yaw) * R(pitch) * front == > [sin(pitch)]
		//								  [sin(yaw)cos(pitch)]


		//上述表示方案是yaw和pitch起始值从0开始的推导
		//front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		//front.y = sin(glm::radians(pitch));
		//front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.SelfNormalize();
		//// Also re-calculate the Right and Up vector
		//right = front.Cross(worldUp).Normalize();  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		//up = right.Cross(front).Normalize();
	}
}
