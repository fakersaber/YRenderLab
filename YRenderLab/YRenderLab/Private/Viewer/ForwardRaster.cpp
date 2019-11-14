#include <Public/Viewer/ForwardRaster.h>
#include <Public/OpenGLRHI/GLAD/glad/glad.h>
#include <Public/OpenGLRHI/GLShader.h>

namespace YRender {
	void ForwardRaster::Draw(){
		glEnable(GL_DEPTH_TEST);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//UpdateShadowMap();
		//UpdateEnvironment();

		// 要放在深度图生成之后
		UpdateUBO();

		modelVec.clear();
		modelVec.push_back(YGM::Transform(1.f));
		//scene->GetRoot()->Accept(This());

		//DrawEnvironment();
	}
	void ForwardRaster::Visit(std::shared_ptr<YObject> obj){

	}

	void ForwardRaster::Visit(std::shared_ptr<BSDF_Diffuse> bsdf) {

	}

	void ForwardRaster::Visit(std::shared_ptr<TriMesh> mesh) {

	}
}

