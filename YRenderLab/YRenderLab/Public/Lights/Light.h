#ifndef _YRENDER_LIGHTS_LIGHT_H_
#define _YRENDER_LIGHTS_LIGHT_H_


#include <Public/Basic/YHeapObject.h>

class Light : public YHeapObject {
public:
	Light() = default;

protected:
	virtual ~Light() = default;
};



#endif