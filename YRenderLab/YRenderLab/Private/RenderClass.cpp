#include <Public/RenderClass.h>



namespace YRender {
	RenderClass::RenderClass() : MainCamera(YRender::New<Camera>()){}
	RenderClass::~RenderClass() {}
}