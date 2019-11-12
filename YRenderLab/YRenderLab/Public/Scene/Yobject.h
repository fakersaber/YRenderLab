#ifndef _YRENDER_SCENE_YOBJECT_H_
#define _YRENDER_SCENE_YOBJECT_H_

#include <Public/Basic/YHeapObject.h>
#include <Public/Basic/Node.h>

#include <string>
#include <unordered_set>

namespace YRender {
	class Component;
	class YObject : public Node<YObject> {
	public:
		YObject(const std::string& objname, std::shared_ptr<YObject> parent = nullptr) :
			name(objname),
			Node(parent)
		{

		}	

	protected:
		YObject() = delete;
		virtual ~YObject() = default;

	public:

		//逻辑线程，shared_ptr安全，不使用值类型
		void AttachComponent(const std::shared_ptr<Component>& component);

		void DetachComponent(const std::shared_ptr<Component>& component);

		const std::unordered_set<std::shared_ptr<Component>>& Getcomponents() const { return components; };


	private:
		friend YHeapObject;
		//shared_ptr的hash是对原生值计算，所以不同的shared对象也能查找
		std::unordered_set<std::shared_ptr<Component>> components;
		std::string name;
	};

	//template<typename ComponentType>
	//inline void Yobject::AttachComponent(const std::shared_ptr<ComponentType>& component){

	//}
}




#endif