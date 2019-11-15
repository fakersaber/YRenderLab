#ifndef _YRENDER_SCENE_TRANSFORM_COMPONENT_H_
#define _YRENDER_SCENE_TRANSFORM_COMPONENT_H_

#include <Public/Scene/Component.h>
#include <Public/YGM/Transform.h>

namespace YRender {
	class TransformComponent : public Component {
		
	public:
		TransformComponent(
			const std::shared_ptr<YObject>& obj,
			const Vector3& pos = Vector3(0.f, 0.f, 0.f),
			const Vector3& scale = Vector3(1.f, 1.f, 1.f)
		) :
			Component(obj),
			position(pos),
			scale(scale)
		{

		}
	protected:
		TransformComponent() = delete;
		virtual ~TransformComponent() = default;

	public:
		const YGM::Transform& GetTransform() const { return transform; }
	private:
		Vector3 position;
		Vector3 scale;
		mutable YGM::Transform transform;
	};
}




#endif