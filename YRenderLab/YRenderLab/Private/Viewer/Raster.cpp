#include <Public/Viewer/Raster.h>

#include <Public/OpenGLRHI/GLAD/glad/glad.h>

#include <Public/Basic/Camera/Camera.h>

#include <Public/Scene/Scene.h>

namespace YRender {
	void Raster::Initial(){
		enviromentGen->Init();
		glGenBuffers(1, &directionalLightsUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, directionalLightsUBO);
		glBufferData(GL_UNIFORM_BUFFER, 32, NULL, GL_DYNAMIC_DRAW); //暂时一个光强和一个光颜色
		glBindBufferBase(GL_UNIFORM_BUFFER, 2, directionalLightsUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);


		//glGenBuffers(1, &environmentUBO);
		//glBindBuffer(GL_UNIFORM_BUFFER, environmentUBO);
		//glBufferData(GL_UNIFORM_BUFFER,)
	}



	void Raster::UpdateEnvironment() {
		enviromentGen->Visit(scene);
	}


	void Raster::UpdateUBO_DirectionalLights(){
		//uniform一个变量在我的显卡上最小数据对其16字节
		glBindBuffer(GL_UNIFORM_BUFFER, directionalLightsUBO);
		Vector3 dir(0.f,0.f,-1.f);
		//Mat4f ProjViewMatrix = camera->GetProjectMatrix() * camera->GetViewMatrix();
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float) * 3, RGBf::White.Data());
		glBufferSubData(GL_UNIFORM_BUFFER, 16, sizeof(float) * 3, dir.Data());
		//glBufferSubData(GL_UNIFORM_BUFFER, 32, 64, ProjViewMatrix.selfTranspose().Data());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void Raster::UpdateUBO_Environment(){

	}


	void Raster::UpdateUBO(){
		UpdateUBO_DirectionalLights();
		UpdateUBO_Environment();
	}


	void Raster::MapUBOToShader(const GLShader& shader) {
		shader.UniformBlockBind("Camera", 0);
		//shader.UniformBlockBind("PointLights", 1);
		shader.UniformBlockBind("DirectionalLights", 2);
		//shader.UniformBlockBind("SpotLights", 3);
		//shader.UniformBlockBind("Environment", 4);
		//shader.UniformBlockBind("SphereLights", 5);
		//shader.UniformBlockBind("DiskLights", 6);
		//shader.UniformBlockBind("AreaLights", 7);
		//shader.UniformBlockBind("CapsuleLights", 8);
	}
}