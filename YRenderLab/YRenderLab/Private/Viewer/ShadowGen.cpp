#include <Public/Viewer/ShadowGen.h>
#include <Public/OpenGLRHI/GlfwWindow.h>
#include <Public/Scene/Scene.h>

ShadowGen::ShadowGen(std::shared_ptr<GlfwWindow> pGLWindow) 
	: 
	pGLWindow(pGLWindow), DepthMapSize(1024) 
{

}

void ShadowGen::init() {
	new (&DirectionalLightShadow) GLShader("Data/shaders/MVP_P3.vs", "");
}



void ShadowGen::Visit(std::shared_ptr<Scene> scene) {

}