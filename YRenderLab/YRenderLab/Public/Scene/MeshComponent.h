#ifndef _YRENDER_SCENE_MESHCOMPONENT_H_
#define _YRENDER_SCENE_MESHCOMPONENT_H_


#include <Public/Scene/Component.h>


//class TriMesh;
class Primitive;
class MeshComponent : public Component {
public:
	MeshComponent(const std::shared_ptr<YObject> obj, const std::shared_ptr<Primitive> primitive);

protected:
	MeshComponent() = delete;
	virtual ~MeshComponent() = default; //#TODO: 被销毁时通知全局容器清除VAO

//public:
//	virtual void Accept() override;

public:
	const std::shared_ptr<Primitive> GetPrimitive() const { return primitive; };

private:
	std::shared_ptr<Primitive> primitive;
};



#endif