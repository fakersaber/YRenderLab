#ifndef _YRENDER_BASIC_YHEAPOBJECT_H_
#define _YRENDER_BASIC_YHEAPOBJECT_H_

#include <memory>

namespace YRender {
	//该类被shared_ptr管理，当赋予原生指针时也能对内存正确计数
	//std::shared_ptr<YHeapObject> p1(new YHeapObject());
	//std::shared_ptr<YHeapObject> p2 = bp1->YHeapObject();
	//若不继承于YHeapObject上方p1与p2会被析构两次
	class YHeapObject : public std::enable_shared_from_this<YHeapObject> {
		template<typename Type,typename ...Args>
		friend const std::shared_ptr<Type> New(Args&& ... args);
	};
}


#endif