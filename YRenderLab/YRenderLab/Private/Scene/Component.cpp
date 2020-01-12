#include <Public/Scene/Component.h>
#include <Public/Scene/Yobject.h>


void Component::InitAfterNew() {
	if (auto owner = GetOwner()) {
		owner->AttachComponent(shared_this<Component>());
	}
}
