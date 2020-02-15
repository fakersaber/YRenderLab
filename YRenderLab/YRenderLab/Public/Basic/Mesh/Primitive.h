#ifndef _YRENDER_BASIC_MESH_PRIMITIVE_H_
#define _YRENDER_BASIC_MESH_PRIMITIVE_H_


#include <Public/Basic/YHeapObject.h>
#include <Public/YGM/Transform.h>

class Raster;
class ShadowGen;

class Primitive : public YHeapObject {
protected:
	Primitive() = default;
	virtual ~Primitive() = default;

public:
	virtual void RenderPrimitive(const std::shared_ptr<Raster>& raster, const YGM::Transform& model) = 0;
	virtual void RenderShadowPrimitive(const std::shared_ptr<ShadowGen>& shadowgen, const YGM::Transform& model) = 0;
};




#endif
