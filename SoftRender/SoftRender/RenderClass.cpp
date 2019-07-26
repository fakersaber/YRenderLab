#include "RenderClass.h"



namespace YRender {

	RenderClass* RenderClass::GetRender() {
		static RenderClass instance;
		return &instance;
	}

	RenderClass::RenderClass() {

	}


	RenderClass::~RenderClass() {

	}
}