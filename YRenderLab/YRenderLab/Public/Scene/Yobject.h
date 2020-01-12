#ifndef _YRENDER_SCENE_YOBJECT_H_
#define _YRENDER_SCENE_YOBJECT_H_

#include <Public/Basic/YHeapObject.h>
#include <Public/Basic/Node.h>

#include <string>
#include <unordered_map>

class Component;
class YObject : public Node<YObject> {
public:
	//当key值为引用类型时，使用reference_wrapper包装后接受右值类型
	using TypeInfoRef = std::reference_wrapper<const std::type_info>;
	struct Hasher {
		std::size_t operator()(TypeInfoRef type) const {
			return type.get().hash_code();
		}
	};

	struct EqualTo {
		bool operator()(TypeInfoRef lhs, TypeInfoRef rhs) const {
			return lhs.get() == rhs.get();
		}
	};
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

	void AttachComponent(const std::shared_ptr<Component> component);

	void DetachComponent(const std::shared_ptr<Component> component);

	const std::unordered_map<TypeInfoRef, std::shared_ptr<Component>, Hasher, EqualTo>& Getcomponents() const { return components; };

	template <typename ComponentType>
	const std::shared_ptr<ComponentType> GetComponent(ComponentType* TemplateParam = nullptr) const;

	//template <typename ComponentType>
	//const std::shared_ptr<ComponentType> GetComponentInChildren()

private:
	friend YHeapObject;

	std::unordered_map<TypeInfoRef, std::shared_ptr<Component>, Hasher, EqualTo> components;

	std::string name;
};


template<typename ComponentType>
inline const std::shared_ptr<ComponentType> YObject::GetComponent(ComponentType* TemplateParam) const {
	static_assert(std::is_base_of_v<Component, ComponentType>, "Component Type Error!");
	auto iter = components.find(typeid(ComponentType));
	if (iter == components.end())
		return nullptr;
	return Cast<ComponentType>(iter->second);
}






#endif