#ifndef _YRENDER_SCENE_SCENE_H_
#define _YRENDER_SCENE_SCENE_H_

#include <Public/Basic/YHeapObject.h>

namespace YRender {
	class YObject;
	class Scene : public YHeapObject {
	public:
		Scene(std::shared_ptr<YObject> root) : root(root) {

		}

	protected:
		virtual ~Scene() = default;

	public:
		const std::shared_ptr<YObject> GetRoot() const { return root; }


	private:
		std::shared_ptr<YObject> root;
	};
}

#endif