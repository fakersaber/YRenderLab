#ifndef _YRENDER_BASIC_MATERIALCOMPONENT_H_
#define _YRENDER_BASIC_MATERIALCOMPONENT_H_

#include <Public/Scene/Component.h>

namespace YRender {
	class Material;
	class MaterialComponent final : public Component {
	public:
		MaterialComponent(std::shared_ptr<YObject> sobj, std::shared_ptr<Material> material)
			: Component(sobj), material(material) { }

	protected:
		virtual ~MaterialComponent() = default;

	public:
		std::shared_ptr<Material> material;
	};
}



#endif