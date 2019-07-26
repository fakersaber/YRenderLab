#ifndef _RENDERCLASS_H
#define _RENDERCLASS_H

#include <iostream>
#include <windows.h>


#include "RenderDevice.h"
#include "GeometryGenerator.h"

namespace YRender {
	class RenderClass {
	public:
		RenderClass(const RenderClass& rhs) = delete;
		RenderClass& operator=(const RenderClass& rhs) = delete;
		static RenderClass* GetRender();

	public:
		bool Initial(HWND hwnd, const int width, const int height);
		void Update();
		void LoadMeshes();
		void Render();

	private:
		~RenderClass();
		RenderClass();
	


	private:
		RenderDevice* _RenderDevice;
	};


}

#endif
