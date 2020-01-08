#ifndef _YRENDER_BASIC_COMPONENT_H_
#define _YRENDER_BASIC_COMPONENT_H_

#include <public/Basic/YHeapObject.h>


	class YObject;
	class Component : public YHeapObject {
	protected:
		Component(const std::shared_ptr<YObject> Owner) : OwnerObj(Owner) {};
		Component() = delete;
		virtual ~Component() = default;

	protected:
		virtual void InitAfterNew() override;
		//virtual void Accept() = 0;
	public:
		const std::shared_ptr<YObject> GetOwner() const { return OwnerObj.lock(); }
	private:
		friend YObject;
		std::weak_ptr<YObject> OwnerObj;
	};






#endif