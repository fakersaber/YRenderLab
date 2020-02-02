#ifndef _YRENDER_BASIC_MATERIAL_H_
#define _YRENDER_BASIC_MATERIAL_H_

#include <Public/Basic/YHeapObject.h>
#include <Public/YGM/Vector2.hpp>
#include <Public/YGM/Vector3.hpp>
#include <Public/YGM/RGB.hpp>
#include <Public/YGM/Math.hpp>


class Material : public YHeapObject {
//public:
//	virtual void UseMaterialTexture() = 0;
//
//
protected:
	Material() = default;
	virtual ~Material() = default;
};




#endif