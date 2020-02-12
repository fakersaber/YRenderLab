#ifndef _YRENDER_SCENE_TRANSFORM_COMPONENT_H_
#define _YRENDER_SCENE_TRANSFORM_COMPONENT_H_

#include <Public/Scene/Component.h>
#include <Public/YGM/Transform.h>

class YObject;

class TransformComponent : public Component {
public:
	const static Vector3 Forward ;
	const static Vector3 Up;
	const static Vector3 Right;
public:
	TransformComponent(const std::shared_ptr<YObject> obj,const YGM::Transform& RelativeTransform = YGM::Transform(1.f));
protected:
	TransformComponent() = delete;
	virtual ~TransformComponent() = default;

public:
	void SetWorldScale(const Vector3& scale);
	void SetRelativeScale(const Vector3& scale);

	const YGM::Transform& GetRelativeTransform() const { return RelativeTransform; }
	const YGM::Transform& GetWorldTransform() const { return WorldTransform; }
private:
	mutable YGM::Transform RelativeTransform;
	mutable YGM::Transform WorldTransform;

};





#endif