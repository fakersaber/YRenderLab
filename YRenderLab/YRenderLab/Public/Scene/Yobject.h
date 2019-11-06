#ifndef _YRENDER_SCENE_YOBJECT_H_
#define _YRENDER_SCENE_YOBJECT_H_

#include <Public/Basic/YHeapObject.h>

namespace YRender {
	class Component;
	class Yobject : YHeapObject {
	public:
		void AddComponent();
		void DestroyComponent();
		void Getcomponent();

	};
}




#endif