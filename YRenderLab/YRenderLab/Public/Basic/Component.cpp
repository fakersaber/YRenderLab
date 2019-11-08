#include <Public/Basic/Component.h>
#include <Public/Scene/Yobject.h>

namespace YRender {
	void Component::InitAfterNew(){
		if (auto owner = GetOwner()) {
			owner->AttachComponent(shared_this<Component>());
		}
	}
}