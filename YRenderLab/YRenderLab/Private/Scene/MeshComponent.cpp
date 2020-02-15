#include <Public/Scene/MeshComponent.h>
#include <Public/Basic/Mesh/Primitive.h>

MeshComponent::MeshComponent(const std::shared_ptr<YObject> obj, const std::shared_ptr<Primitive> primitive) :Component(obj),
	primitive(primitive) {

}