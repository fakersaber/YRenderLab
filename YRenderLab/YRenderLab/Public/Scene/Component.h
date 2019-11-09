#ifndef _YRENDER_BASIC_COMPONENT_H_
#define _YRENDER_BASIC_COMPONENT_H_

#include <public/Basic/YHeapObject.h>


namespace YRender {
	class YObject;
	class Component : public YHeapObject {
	public:
		//必须指定负载的Object
		Component() = delete;
		Component(const std::shared_ptr<YObject>& Owner) : OwnerObj(Owner) {};
		virtual ~Component() = default;

	protected:
		virtual void InitAfterNew() override;

	public:
		const std::shared_ptr<YObject> GetOwner() const { return OwnerObj.lock(); }
	private:
		friend YObject;
		std::weak_ptr<YObject> OwnerObj;
	};
}





#endif