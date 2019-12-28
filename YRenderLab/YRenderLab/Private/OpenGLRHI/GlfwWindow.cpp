#include <Public/OpenGLRHI/GlfwWindow.h>
#include <Public/OpenGLRHI/GLAD/glad/glad.h>
#include <Public/Viewer/ForwardRaster.h>
#include <Public/Scene/AssimpLoader.h>
#include <Public/Scene/Scene.h>
#include <Public/Basic/Mesh/TriMesh.h>
#include <Public/Basic/Image/Image.h>
#include <Public/YCore.h>

namespace YRender {
	bool GlfwWindow::Initial(const int width, const int height) {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, GlfwWindow::mouse_callback);
		//glfwSetScrollCallback(window, GlfwWindow::scroll_callback);

		//初始化glad
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return false;
		}


		//camera可以从pipline中获得，可以不写在窗口类中
		MainCamera = YRender::New<Camera>();
		//创建场景、管线、相机
		auto Root = YRender::AssimpLoader::Load("D:/YRenderLab/YRenderLab/YRenderLab/Data/module/revolver/source/Test.FBX");
		auto Scene = YRender::New<YRender::Scene>(Root,MainCamera);
		ForwardPipline = YRender::New<ForwardRaster>(Scene, shared_this<GlfwWindow>());
		ForwardPipline->Initial();
		MainCamera->Initial(width, height);
		return true;
	}

	void GlfwWindow::Run(){
		while (!glfwWindowShouldClose(window)) {
			double beginTime = glfwGetTime();
			ProcessInput(window);
			beginTime = glfwGetTime();

			ForwardPipline->Draw();

			deltaTime = static_cast<float>(glfwGetTime() - beginTime);
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	VAO GlfwWindow::GetVAO(std::shared_ptr<TriMesh> mesh){
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

	VAO GlfwWindow::GetVAO(GlfwWindow::VAOTYPE CurType)
	{
		switch (CurType) {
		case VAOTYPE::Screen:
			return VAO(CoreDefine::data_ScreenVertices, sizeof(CoreDefine::data_ScreenVertices), { 2,2 });
		default:
			printf("GetVAO Error! %d", CurType);
			return VAO();
		}
	}

	GLTexture GlfwWindow::GetTexture(std::shared_ptr<Image> img){
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

	void GlfwWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
		YCore::GetCore()->GetGLWindow()->GetCamera()->SetWH(width, height);
	}

	void GlfwWindow::mouse_callback(GLFWwindow* window, double xpos, double ypos){
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
}
