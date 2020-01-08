#ifndef _YRENDER_VIEWER_RASTER_H_
#define _YRENDER_VIEWER_RASTER_H_

#include <Public/Basic/YHeapObject.h>
#include <Public/OpenGLRHI/GLShader.h>


class Scene;
class Camera;
class EnviromentGen;
class GlfwWindow;

class Raster : public YHeapObject {
protected:
	Raster(std::shared_ptr<Scene> scene, std::shared_ptr<EnviromentGen> enviromentGen, std::shared_ptr<GlfwWindow> pGLWindow)
		:
		scene(scene),
		enviromentGen(enviromentGen),
		pGLWindow(pGLWindow)
	{

	};
	virtual ~Raster() = default;

public:
	virtual void Draw() = 0;
	virtual void Initial();

protected:
	void MapUBOToShader(const GLShader& shader);
	void UpdateUBO_Camera();
	void UpdateUBO_DirectionalLights();
	void UpdateUBO_Environment();
public:


protected:
	std::shared_ptr<Scene> scene;
	std::shared_ptr<GlfwWindow> pGLWindow;
	std::shared_ptr<EnviromentGen> enviromentGen;
private:
	unsigned int directionalLightsUBO;
	unsigned int environmentUBO;
	unsigned int cameraUBO;


	//unsigned int StandardMaterialPassUBO;
	//unsigned int EnviromentPassUBO;
	//unsigned int TaaPassUBO;
	//unsigned int LightPassUBO;
};


#endif