#ifndef _YRENDER_SCENE_YOBJECT_H_
#define _YRENDER_SCENE_YOBJECT_H_

#include <Public/Basic/YHeapObject.h>

#include <string>
#include <unordered_set>

namespace YRender {
	class Component;
	class YObject : public YHeapObject {
	public:
		YObject(const std::string& objname = "DefaultObj") : name(objname) {}
		
		virtual ~YObject() = default;

		YObject() = delete;

	public:
		void AttachComponent(const std::shared_ptr<Component>& component);

		void DetachComponent(const std::shared_ptr<Component>& component);

		const std::unordered_set<std::shared_ptr<Component>>& Getcomponent() const { return components; };


	private:
		std::unordered_set<std::shared_ptr<Component>> components;
		std::string name;
	};

	//template<typename ComponentType>
	//inline void Yobject::AttachComponent(const std::shared_ptr<ComponentType>& component){

	//}
}




#endif