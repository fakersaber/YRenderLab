#include <Public/Basic/Camera/Camera.h>

#include <Public/OpenGLRHI/GLAD/glad/glad.h>

const float Camera::ASPECT_WH = 1.333f;
const float Camera::NEAR_PLANE = 0.1f;
const float Camera::FAR_PLANE = 100.0f;
const float Camera::YAW = 0.f;  //Clockwise rotation is the nagative direction
const float Camera::PITCH = 0.f;
const float Camera::FOV = 50.0f;

Camera::Camera(
	const Vector3& pos,
	float yaw,
	float pitch,
	float aspect,
	float nearPlane,
	float farPlane,
	const Vector3& up,
	ENUM_ProjectType projectionMode
)
	:
	position(pos),
	yaw(yaw),
	pitch(pitch),
	aspect_wh(aspect),
	nearPlane(nearPlane),
	farPlane(farPlane),
	worldup(up),
	project_mode(projectionMode),
	fov(FOV)
{

	Camera::updateCameraVectors();
}


void Camera::Initial(int w, int h) {

	aspect_wh = static_cast<float>(w) / static_cast<float>(h);
}

Mat4f Camera::GetViewMatrix() const {

	Mat4f worldToCamera;
	worldToCamera(0, 0) = right.x;
	worldToCamera(0, 1) = right.y;
	worldToCamera(0, 2) = right.z;

	worldToCamera(1, 0) = up.x;
	worldToCamera(1, 1) = up.y;
	worldToCamera(1, 2) = up.z;

	worldToCamera(2, 0) = -front.x;
	worldToCamera(2, 1) = -front.y;
	worldToCamera(2, 2) = -front.z;

	worldToCamera(0, 3) = -right.Dot(position);
	worldToCamera(1, 3) = -up.Dot(position);
	worldToCamera(2, 3) = front.Dot(position);

	return worldToCamera;
}

Mat4f Camera::GetInvViewMatrix() const {

	Mat4f CameraToWorld;
	CameraToWorld(0, 0) = right.x;
	CameraToWorld(1, 0) = right.y;
	CameraToWorld(2, 0) = right.z;

	CameraToWorld(0, 1) = up.x;
	CameraToWorld(1, 1) = up.y;
	CameraToWorld(2, 1) = up.z;

	CameraToWorld(0, 2) = -front.x;
	CameraToWorld(1, 2) = -front.y;
	CameraToWorld(2, 2) = -front.z;

	CameraToWorld(0, 3) = position.x;
	CameraToWorld(1, 3) = position.y;
	CameraToWorld(2, 3) = position.z;

	return CameraToWorld;
}


Mat4f Camera::GetProjectMatrix() const {

	const float tanHalfFovy = tan(YGM::Math::Radians(fov * 0.5f));
	Mat4f PerspectiveMat(0.f);
	PerspectiveMat(0, 0) = 1.f / (tanHalfFovy * aspect_wh);
	PerspectiveMat(1, 1) = 1.f / tanHalfFovy;
	PerspectiveMat(2, 2) = -(nearPlane + farPlane) / (farPlane - nearPlane);
	PerspectiveMat(2, 3) = -(2.f * nearPlane * farPlane) / (farPlane - nearPlane);
	PerspectiveMat(3, 2) = -1.f;
	return PerspectiveMat;
}

void Camera::SetWH(int w, int h) {
	aspect_wh = static_cast<float>(w) / static_cast<float>(h);
}


void Camera::SetCameraPose(const Vector3& pos, float yaw, float pitch) {
	this->position = pos;
	this->yaw = yaw;
	this->pitch = pitch;
	updateCameraVectors();
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

	//u神的运算方式与opengl运算代码相同,yaw的起始值为-PI/2,实际变为了3PI/2 - x, 即- x - PI/2
	//								 [-sin(yaw), -cos(yaw)sin(pitch),  -cos(yaw)cos(pitch)]
	// R(yaw - PI/2) * R(pitch) == > [0,         cos(pitch),          -sin(pitch)]
	//								 [cos(yaw), -sin(yaw)sin(pitch),  -sin(yaw)cos(pitch)]
	//接着同理表示front向量
	//								  [cos(yaw)cos(pitch)]
	// R(yaw) * R(pitch) * front == > [sin(pitch)]
	//								  [sin(yaw)cos(pitch)]
	//上述表示方案是yaw从-PI/2开始的推导。

	front.x = -sin(YGM::Math::Radians(yaw)) * cos(YGM::Math::Radians(pitch));
	front.y = sin(YGM::Math::Radians(pitch));
	front.z = -cos(YGM::Math::Radians(yaw)) * cos(YGM::Math::Radians(pitch));

	//front.x = cos(YGM::Math::Radians(yaw)) * cos(YGM::Math::Radians(pitch));
	//front.y = sin(YGM::Math::Radians(pitch));
	//front.z = sin(YGM::Math::Radians(yaw)) * cos(YGM::Math::Radians(pitch));

	front.SelfNormalize();
	right = front.Cross(worldup).Normalize();  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = right.Cross(front).Normalize();
}


void Camera::ProcessKeyboard(ENUM_Movement direction, float deltaTime) {
	float velocity = 100.f * deltaTime;  //ready change for deltaTime
	switch (direction) {
	case ENUM_Movement::MOVE_FORWARD:
		position += front * velocity;
		break;
	case ENUM_Movement::MOVE_BACKWARD:
		position -= front * velocity;
		break;
	case ENUM_Movement::MOVE_LEFT:
		position -= right * velocity;
		break;
	case ENUM_Movement::MOVE_RIGHT:
		position += right * velocity;
		break;
	case ENUM_Movement::MOVE_UP:
		position += up * velocity;
		break;
	case ENUM_Movement::MOVE_DOWN:
		position -= up * velocity;
		break;
	}
}



// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
	xoffset *= 0.2f;
	yoffset *= 0.2f;

	yaw += xoffset;
	pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	//if (constrainPitch)
	//{
	if (pitch > 89.f)
		pitch = 89.f;
	else if (pitch < -89.f)
		pitch = -89.f;
	//}

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

std::vector<Vector3> Camera::Corners() const {

	YGM::Transform NdcToWorld((GetProjectMatrix() * GetViewMatrix()).Inverse());

	const Vector3 nCorners[8] = {
	{-1.f,-1.f,-1.f},
	{-1.f,-1.f, 1.f},
	{-1.f, 1.f,-1.f},
	{-1.f, 1.f, 1.f},
	{ 1.f,-1.f,-1.f},
	{ 1.f,-1.f, 1.f},
	{ 1.f, 1.f,-1.f},
	{ 1.f, 1.f, 1.f},
	};


	std::vector<Vector3> CornersWorld;
	for (int i = 0; i < 8; i++) {
		CornersWorld.emplace_back(NdcToWorld.TrasformPoint(nCorners[i]));
	}

	return CornersWorld;
}
