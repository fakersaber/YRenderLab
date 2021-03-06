#ifndef _YRENDER_BASIC_MATERIALCOMPONENT_H_
#define _YRENDER_BASIC_MATERIALCOMPONENT_H_

#include <Public/Scene/Component.h>


class Material;
class MaterialComponent final : public Component {
public:
	MaterialComponent(std::shared_ptr<YObject> sobj, std::shared_ptr<Material> material)
		: Component(sobj), material(material) { }

protected:
	virtual ~MaterialComponent() = default;

public:
	const std::shared_ptr<Material> GetMaterial() const { return material; };

private:
	std::shared_ptr<Material> material;
};




#endif