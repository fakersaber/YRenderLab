#include "RenderClass.h"



namespace YRender {

	RenderClass* RenderClass::GetRender() {
		static RenderClass instance;
		return &instance;
	}

	RenderClass::RenderClass() {
		_RenderDevice = new RenderDevice();
	}

	bool RenderClass::Initial(HWND hwnd, const int width, const int height) {
		if (!_RenderDevice->Initial(hwnd, width, height))
		{
			std::cout << "Create Device Failed" << std::endl;
			return false;
		}

		GeometryGenerator::CreateBox(2.0f, 2.0f, 2.0f, this->Mesh);

	}

	RenderClass::~RenderClass() {
		delete _RenderDevice;
	}


	void RenderClass::Update() {

	}

	void RenderClass::Render() {

	}


	void RenderClass::LoadMeshes() {

	}


}