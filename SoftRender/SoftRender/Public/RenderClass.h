#ifndef _YRENDER_RENDERCLASS_H
#define _YRENDER_RENDERCLASS_H

#include <iostream>
#include <Public/RenderDevice.h>
#include <Public/GeometryGenerator.h>
#include <Public/YGM/Matrix4.hpp>
#include <Public/YGM/Transform.h>


namespace YRender {
	class RenderClass {
	public:
		RenderClass(const RenderClass& rhs) = delete;
		RenderClass& operator=(const RenderClass& rhs) = delete;
		static RenderClass* GetRender();

	public:
		bool Initial(HWND hwnd, const int width, const int height);
		void Update();
		//void LoadMeshes();
		void Render();
	
	private:
		~RenderClass();
		RenderClass();

	private:
		void DrawIndexed(const MeshData& mesh);


	private:
		RenderDevice* _RenderDevice;
		MeshData Mesh;
	};


}

#endif
