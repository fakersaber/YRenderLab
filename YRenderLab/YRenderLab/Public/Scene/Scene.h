#ifndef _YRENDER_SCENE_SCENE_H_
#define _YRENDER_SCENE_SCENE_H_

#include <Public/Basic/YHeapObject.h>
#include <Public/Basic/Camera/Camera.h>

namespace YRender {
	class YObject;
	class Scene : public YHeapObject {
	public:
		Scene(std::shared_ptr<YObject> root, std::shared_ptr<Camera> camera) : 
			root(root),
			camera(camera)
		{
		}

	protected:
		virtual ~Scene() = default;

	public:
		const std::shared_ptr<YObject> GetRoot() const { return root; }
		const std::shared_ptr<Camera> GetCamera() const { return camera; }

	private:
		std::shared_ptr<YObject> root;
		std::shared_ptr<Camera> camera;
	};
}

#endif