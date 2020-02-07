#ifndef _YRENDER_SCENE_LIGHTCOMPONENT_H_
#define _YRENDER_SCENE_LIGHTCOMPONENT_H_

#include <Public/Lights/Light.h>
#include <Public/Scene/Component.h>

class LightComponent : public Component {
	LightComponent(const std::shared_ptr<YObject> obj, const std::shared_ptr<Light> LightInstance) :
		Component(obj),
		LightInstance(LightInstance)
	{
	}

protected:
	LightComponent() = delete;
	virtual ~LightComponent() = default; 


public:
	std::shared_ptr<Light> GetLight() const { return LightInstance; };

private:
	std::shared_ptr<Light> LightInstance;
};


#endif