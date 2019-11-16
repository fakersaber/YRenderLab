#ifndef _YRENDER_RENDERCLASS_H
#define _YRENDER_RENDERCLASS_H

#include <Public/RenderDevice.h>
#include <Public/Basic/BasicCore.h>



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
			return *MainCamera;
		};


	protected:
		std::unique_ptr<RenderDevice> _RenderDevice;
		std::shared_ptr<Camera> MainCamera;
		MeshData Mesh;
		Mat4f NdcToScreen;



		int32_t width;
		int32_t height;
	};
}

#endif
