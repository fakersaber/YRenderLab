#ifndef _YRENDER_RENDERCLASS_H
#define _YRENDER_RENDERCLASS_H

#include <iostream>
#include <Public/RenderDevice.h>
#include <Public/GeometryGenerator.h>
#include <Public/YGM/Matrix4.hpp>
#include <Public/YGM/Transform.h>
#include <Public/Camera.h>


namespace YRender {
	class RenderClass 
	{
	public:
		virtual bool Initial(HWND hwnd, const int width, const int height) = 0;
		virtual void Tick() = 0;
		virtual void Render() = 0;

	private:
		RenderClass(const RenderClass& rhs) = delete;
		RenderClass& operator=(const RenderClass& rhs) = delete;

	protected:
		virtual ~RenderClass();
		RenderClass();

	protected:
		RenderDevice* _RenderDevice;
		Camera MainCamera;
		MeshData Mesh;
		Mat4f NdcToScreen;
	};
}

#endif
