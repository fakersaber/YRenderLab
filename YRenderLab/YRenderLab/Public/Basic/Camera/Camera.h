#ifndef _YRENDER_CAMERA_H
#define _YRENDER_CAMERA_H

#include <Public/YGM/Transform.h>
#include <Public/Basic/YHeapObject.h>

#include  <vector>

class Camera/* : public YHeapObject */{
public:
	enum class ENUM_ProjectType : uint8_t {
		Orthogonal = 1,
		Perspective = 2
	};

	enum class ENUM_Movement : uint8_t {
		MOVE_FORWARD,
		MOVE_BACKWARD,
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_UP,
		MOVE_DOWN
	};

public:
	Camera(
		const Vector3& pos = Vector3(0.f, 0.f, 0.f),
		float aspect = Camera::ASPECT_WH,
		float nearPlane = Camera::NEAR_PLANE,
		float farPlane = Camera::FAR_PLANE,
		const Vector3& up = Vector3(0.f, 1.f, 0.f),
		ENUM_ProjectType projectionMode = ENUM_ProjectType::Perspective
	);
	~Camera() = default;

public:
	void Initial(int w, int h);

	Mat4f GetViewMatrix() const; /*{ return YGM::Transform::LookAt(position, position + front, up).GetMatrix(); }*/

	Mat4f GetInvViewMatrix() const; /*{ return YGM::Transform::LookAt(position, position + front, up).GetInvMatrix(); }*/

	Mat4f GetProjectMatrix() const; /*{ return YGM::Transform::Perspective(fov, aspect_wh, nearPlane, farPlane).GetMatrix(); }*/

	inline float GetAspect() const { return aspect_wh; }

	inline float GetFov() const { return fov; }

	inline float GetNearPlane() const { return nearPlane; }

	inline float* GetNearPlaneData() { return &nearPlane; }

	inline float GetFarPlane() const { return farPlane; }

	inline float* GetFarPlaneData() { return &farPlane; }

	inline float* GetFovData() { return &fov; }

	inline float* GetAspectData() { return &aspect_wh; }

	inline const Vector3& GetForward() const { return front; }

	void SetCameraPose(const Vector3& pos, float yaw, float pitch);

	ENUM_ProjectType GetCameraMode() { return project_mode; }

	const Vector3& GetCameraPos() const { return position; }

	void ProcessKeyboard(ENUM_Movement event, float deltaTime);

	void ProcessMouseMovement(float xoffset, float yoffset);

	std::vector<Vector3> Corners() const;

	void updateCameraVectors();

	void SetWH(int w, int h);

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
	ENUM_ProjectType project_mode;

	// Default camera values
	static const float ASPECT_WH;
	static const float NEAR_PLANE;
	static const float FAR_PLANE;
	static const float YAW;
	static const float PITCH;
	static const float FOV;
};

#endif