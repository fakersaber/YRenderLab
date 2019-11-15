#include <Public/Viewer/Raster.h>

#include <Public/OpenGLRHI/GLAD/glad/glad.h>
#include <Public/OpenGLRHI/GLShader.h>

#include <Public/Scene/Scene.h>

namespace YRender {
	void Raster::Initial(){
		glGenBuffers(1, &directionalLightsUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, directionalLightsUBO);
		glBufferData(GL_UNIFORM_BUFFER, 784, NULL, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 2, directionalLightsUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}



	void Raster::MapUBOToShader(const GLShader& shader){
		shader.UniformBlockBind("Camera", 0);
		shader.UniformBlockBind("PointLights", 1);
		shader.UniformBlockBind("DirectionalLights", 2);
		shader.UniformBlockBind("SpotLights", 3);
		shader.UniformBlockBind("Environment", 4);
	}

	void Raster::UpdateUBO_DirectionalLights(){

	}


	void Raster::UpdateUBO(){
		UpdateUBO_DirectionalLights();
	}
}