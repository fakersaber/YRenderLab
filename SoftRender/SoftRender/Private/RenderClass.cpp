﻿#include <Public/RenderClass.h>



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

		GeometryGenerator::CreateBox(this->Mesh);

		return true;
	}

	RenderClass::~RenderClass() {
		delete _RenderDevice;
	}


	void RenderClass::Update() {

	}

	void RenderClass::Render() {
		_RenderDevice->DrawPixel(400, 300);
		_RenderDevice->DrawPixel(700, 500);
		_RenderDevice->DrawFrameBuffer();
	}


	void RenderClass::LoadMeshes() {

	}


}