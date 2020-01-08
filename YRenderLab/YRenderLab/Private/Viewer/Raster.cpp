#include <Public/Viewer/Raster.h>
#include <Public/OpenGLRHI/GlfwWindow.h>
#include <Public/OpenGLRHI/GLAD/glad/glad.h>
#include <Public/Basic/Camera/Camera.h>
#include <Public/Scene/Scene.h>
#include <Public/OpenGLRHI/GlfwWindow.h>
#include <Public/Viewer/EnviromentGen.h>


void Raster::Initial() {
	enviromentGen->Init();

	glGenBuffers(1, &cameraUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, cameraUBO);
	glBufferData(GL_UNIFORM_BUFFER, 160, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, cameraUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);


	glGenBuffers(1, &directionalLightsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, directionalLightsUBO);
	glBufferData(GL_UNIFORM_BUFFER, 48, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, directionalLightsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);


	glGenBuffers(1, &environmentUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, environmentUBO);
	glBufferData(GL_UNIFORM_BUFFER, 32, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 4, environmentUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}



void Raster::UpdateUBO_DirectionalLights() {
	auto directionalLightIdx = 1;
	Vector3 dir(1.f, 0.f, 0.f);
	glBindBuffer(GL_UNIFORM_BUFFER, directionalLightsUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 4, &directionalLightIdx);
	glBufferSubData(GL_UNIFORM_BUFFER, 16, sizeof(float) * 3, RGBf::White.Data());
	glBufferSubData(GL_UNIFORM_BUFFER, 32, sizeof(float) * 3, dir.Data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Raster::UpdateUBO_Environment() {
	glBindBuffer(GL_UNIFORM_BUFFER, environmentUBO);

	auto environment = scene->GetEnviromentImg();
	if (!environment) {
		auto color = RGBf(0.f);
		float intensity = 0;
		bool haveSkybox = false;
		bool haveEnvironment = false;
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 12, color.Data());
		glBufferSubData(GL_UNIFORM_BUFFER, 12, 4, &intensity);
		glBufferSubData(GL_UNIFORM_BUFFER, 16, 1, &haveSkybox);
		glBufferSubData(GL_UNIFORM_BUFFER, 20, 1, &haveEnvironment);
	}
	else {
		float intensity = 1.f;
		bool haveSkybox = true;
		bool haveEnvironment = true;
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 12, RGBf::White.Data());
		glBufferSubData(GL_UNIFORM_BUFFER, 12, 4, &intensity);
		glBufferSubData(GL_UNIFORM_BUFFER, 16, 1, &haveSkybox);
		glBufferSubData(GL_UNIFORM_BUFFER, 20, 1, &haveEnvironment);
	}

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


void Raster::UpdateUBO_Camera() {
	glBindBuffer(GL_UNIFORM_BUFFER, cameraUBO);
	auto camera = scene->GetCamera();

	glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, camera->GetViewMatrix().Transpose().Data());
	glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, camera->GetProjectMatrix().Transpose().Data());
	glBufferSubData(GL_UNIFORM_BUFFER, 128, 12, camera->GetCameraPos().Data());
	glBufferSubData(GL_UNIFORM_BUFFER, 140, 4, camera->GetNearPlaneData());
	glBufferSubData(GL_UNIFORM_BUFFER, 144, 4, camera->GetFarPlaneData());
	glBufferSubData(GL_UNIFORM_BUFFER, 148, 4, camera->GetFovData());
	glBufferSubData(GL_UNIFORM_BUFFER, 152, 4, camera->GetAspectData());

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


//在某些shader中会因为不存在某些uniformBlock而抛出GL_INVALID_VALUE错误，无视
void Raster::MapUBOToShader(const GLShader& shader) {
	shader.UniformBlockBind("Camera", 0);
	//shader.UniformBlockBind("PointLights", 1);
	shader.UniformBlockBind("DirectionalLights", 2);
	//shader.UniformBlockBind("SpotLights", 3);
	shader.UniformBlockBind("Environment", 4);
	//shader.UniformBlockBind("SphereLights", 5);
	//shader.UniformBlockBind("DiskLights", 6);
	//shader.UniformBlockBind("AreaLights", 7);
	//shader.UniformBlockBind("CapsuleLights", 8);
}

