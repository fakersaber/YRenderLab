#ifndef _YRENDER_RENDERCLASS_H
#define _YRENDER_RENDERCLASS_H

#include <iostream>
#include <Public/RenderDevice.h>
#include <Public/Basic/Mesh/GeometryGenerator.h>
#include <Public/Basic/Camera/Camera.h>
#include <Public/YGM/Matrix4.hpp>
#include <Public/YGM/Transform.h>



namespace YRender {
	class RenderClass 
	{
	public:
		RenderClass(const RenderClass& rhs) = delete;
		RenderClass& operator=(const RenderClass& rhs) = delete;

	public:
		virtual bool Initial(const int width, const int height) = 0;
		virtual void Tick() = 0;
		virtual void Render() = 0;

	public:
		RenderClass();
		virtual ~RenderClass();

		Camera& GetCamera(){
			return this->MainCamera;
		};


	protected:
		RenderDevice* _RenderDevice;
		Camera MainCamera;
		MeshData Mesh;
		Mat4f NdcToScreen;
		int32_t width;
		int32_t height;
	};
}

#endif
