#include <Public/OpenGLRHI/GlfwWindow.h>
#include <Public/OpenGLRHI/GLAD/glad/glad.h>
#include <Public/Viewer/ForwardRaster.h>
#include <Public/Viewer/DeferredRaster.h>

#include <Public/Scene/AssimpLoader.h>
#include <Public/Scene/Yobject.h>
#include <Public/Scene/Scene.h>
#include <Public/Lights/DirectionalLight.h>
#include <Public/Scene/LightComponent.h>
#include <Public/Scene/TransformComponent.h>

#include <Public/Basic/Image/Image.h>
#include <Public/YCore.h>

#include <Public/Scene/MeshComponent.h>
#include <Public/Basic/Mesh/Cube.h>
#include <Public/Basic/Mesh/Plane.h>
#include <Public/Basic/Mesh/TriMesh.h>


#include <Public/Basic/BSDF_Emission.h>
#include <Public/Basic/MaterialComponent.h>

bool GlfwWindow::Initial(const int width, const int height){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(width, height, "YRender", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	lastX = width * 0.5f;
	lastY = height * 0.5f;
	this->width = width;
	this->height = height;
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, GlfwWindow::mouse_callback);
	//glfwSetScrollCallback(window, GlfwWindow::scroll_callback);

	//初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	//initial StaticMeshContainer
	{
		StaticMeshContainer.emplace(CoreDefine::StaticVAOType::Screen, VAO(CoreDefine::data_ScreenVertices, sizeof(CoreDefine::data_ScreenVertices), { 2,2 }));

		auto StaticCube = New<Cube>();
		StaticMeshContainer.emplace(
			CoreDefine::StaticVAOType::Cube,
			VAO(
				{
					{ StaticCube->GetPositions().data()->Data(), static_cast<unsigned int>(StaticCube->GetPositions().size() * 3 * sizeof(float)), 3 } ,
					{ StaticCube->GetNormals().data()->Data(), static_cast<unsigned int>(StaticCube->GetNormals().size() * 3 * sizeof(float)), 3 },
					{ StaticCube->GetTexcoords().data()->Data(), static_cast<unsigned int>(StaticCube->GetTexcoords().size() * 2 * sizeof(float)), 2 }
				},
				StaticCube->GetIndice().data(),
				static_cast<unsigned int>(StaticCube->GetIndice().size() * sizeof(unsigned int))
			)
		);

		//
		auto StaicPlane = New<Plane>();
		StaticMeshContainer.emplace(
			CoreDefine::StaticVAOType::Plane,
			VAO(
				{
					{ StaicPlane->GetPositions().data()->Data(), static_cast<unsigned int>(StaicPlane->GetPositions().size() * 3 * sizeof(float)), 3 } ,
					{ StaicPlane->GetNormals().data()->Data(), static_cast<unsigned int>(StaicPlane->GetNormals().size() * 3 * sizeof(float)), 3 },
					{ StaicPlane->GetTexcoords().data()->Data(), static_cast<unsigned int>(StaicPlane->GetTexcoords().size() * 2 * sizeof(float)), 2 }
				},
				StaicPlane->GetIndice().data(),
				static_cast<unsigned int>(StaicPlane->GetIndice().size() * sizeof(unsigned int))
			)
		);
	}

	//#TODO：camera可以从pipline中获得，可以不写在窗口类中
	MainCamera = New<Camera>();
	MainCamera->Initial(width, height);

	auto Root = AssimpLoader::Load("Data/module/Cerberus_by_Andrew_Maximov/TargetModule.FBX");

	//创建光源
	{
		auto light_0 = New<YObject>("light_0", Root);
		New<TransformComponent>(light_0);
		New<LightComponent>(light_0, New<DirectionalLight>(RGBf::White, 1.f));
	}

	//创建Plane
	{
		auto Plane_0 = New<YObject>("Plane_0", Root);
		New<MeshComponent>(Plane_0, New<Plane>());
		auto PlaneTransform = New<TransformComponent>(Plane_0);
		//顺序必须是缩放，旋转，平移
		PlaneTransform->SetWorldScale(Vector3(20.f, 20.f, 20.f));
		PlaneTransform->RotateX(90.f);
		PlaneTransform->SetWorldTranslate(Vector3(0.f, 0.f, -60.f));
		
		New<MaterialComponent>(Plane_0, New<BSDF_Emission>());
	}

	Root->PrintNode();

	//创建场景，管线
	auto scene = New<Scene>(Root, MainCamera, New<Image>("Data/module/Arches_E_PineTree/Arches_E_PineTree_3k.hdr"));
	//RenderRaster = New<ForwardRaster>(scene, shared_this<GlfwWindow>());
	RenderRaster = New<DeferredRaster>(scene, shared_this<GlfwWindow>());
	RenderRaster->Initial();

	

	return true;
}

void GlfwWindow::Run() {
	while (!glfwWindowShouldClose(window)) {
		double beginTime = glfwGetTime();
		ProcessInput(window);
		beginTime = glfwGetTime();

		RenderRaster->Draw();

		deltaTime = static_cast<float>(glfwGetTime() - beginTime);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}

VAO GlfwWindow::GetVAO(std::shared_ptr<TriMesh> mesh) {
	auto TargetVAO = mesh2VAO.find(mesh);
	if (TargetVAO == mesh2VAO.end()) {
		std::vector<VAO::VBO_DataPatch> Mesh_VAO_DataPatch = {
		{mesh->GetPositions().data()->Data(), static_cast<unsigned int>(mesh->GetPositions().size() * 3 * sizeof(float)), 3},
		{ mesh->GetNormals().data()->Data(), static_cast<unsigned int>(mesh->GetNormals().size() * 3 * sizeof(float)), 3 },
		{ mesh->GetTexcoords().data()->Data(), static_cast<unsigned int>(mesh->GetTexcoords().size() * 2 * sizeof(float)), 2 },
		{ mesh->GetTangents().data()->Data(), static_cast<unsigned int>(mesh->GetTangents().size() * 3 * sizeof(float)), 3 } };
		VAO VAO_P3N3T2T3_Mesh(Mesh_VAO_DataPatch, mesh->GetIndice().data(), static_cast<unsigned int>(mesh->GetIndice().size() * sizeof(unsigned int)));
		mesh2VAO[mesh] = VAO_P3N3T2T3_Mesh;
		return VAO_P3N3T2T3_Mesh;
	}
	else {
		return TargetVAO->second;
	}
}

VAO GlfwWindow::GetVAO(CoreDefine::StaticVAOType CurType)
{
	auto iter = StaticMeshContainer.find(CurType);
	if (iter != StaticMeshContainer.end()) {
		return iter->second;
	}
	assert(false);
	printf("GetVAO Error! %d", CurType);
	return VAO();

}

GLTexture GlfwWindow::GetTexture(std::shared_ptr<Image> img) {
	auto target = img2tex.find(img);
	if (target == img2tex.end()) {
		GLTexture CurTextrue(img);
		img2tex[img] = CurTextrue;
		return CurTextrue;
	}
	else {
		return target->second;
	}
}

std::shared_ptr<Camera> GlfwWindow::GetCamera() const {
	return MainCamera;
}

std::shared_ptr<Raster> GlfwWindow::GetRaster() const {
	return RenderRaster;
}

void GlfwWindow::UpdateViewPort(unsigned int width, unsigned int height) {
	this->width = width;
	this->height = height;
}

GlfwWindow::GlfwWindow()
	:firstFlag(true)
{

}


void GlfwWindow::ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		MainCamera->ProcessKeyboard(Camera::ENUM_Movement::MOVE_FORWARD, GetDeltaTime());
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		MainCamera->ProcessKeyboard(Camera::ENUM_Movement::MOVE_BACKWARD, GetDeltaTime());
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		MainCamera->ProcessKeyboard(Camera::ENUM_Movement::MOVE_LEFT, GetDeltaTime());
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		MainCamera->ProcessKeyboard(Camera::ENUM_Movement::MOVE_RIGHT, GetDeltaTime());
}

auto GlfwWindow::GetViewPortW() -> decltype(width)
{
	return width;
}

auto GlfwWindow::GetViewPortH() -> decltype(height)
{
	return height;
}

void GlfwWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.

	//最小化窗口无视掉
	if (width == 0 || height == 0)
		return;

	GLFBO::UseDefault();
	glViewport(0, 0, width, height);
	auto GLWindowPtr = YCore::GetCore()->GetGLWindow();
	GLWindowPtr->GetCamera()->SetWH(width, height);
	GLWindowPtr->UpdateViewPort(width, height);
	GLWindowPtr->RenderRaster->Resize(width, height);
}

void GlfwWindow::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	auto GlWindow = YCore::GetCore()->GetGLWindow();
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		float xoffset = 0.f;
		float yoffset = 0.f;
		if (!GlWindow->firstFlag) {
			//不是第一次按住鼠标右键才计算差值
			xoffset = GlWindow->lastX - static_cast<float>(xpos);
			yoffset = GlWindow->lastY - static_cast<float>(ypos);
		}
		GlWindow->lastX = static_cast<float>(xpos);
		GlWindow->lastY = static_cast<float>(ypos);
		GlWindow->firstFlag = false;
		GlWindow->MainCamera->ProcessMouseMovement(xoffset, yoffset);
	}
	else {
		GlWindow->firstFlag = true;
	}
}

//void GlfwWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

//}

//void GlfwWindow::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){

//}
