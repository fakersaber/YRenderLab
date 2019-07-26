#ifndef _RENDERCLASS_H
#define _RENDERCLASS_H

#include <iostream>

//该类是做渲染运算，将数据传输到Device中，不管Device是什么接口，只需要DrawPix

namespace YRender {
	class RenderClass {
	public:
		RenderClass(const RenderClass& rhs) = delete;
		RenderClass& operator=(const RenderClass& rhs) = delete;
		static RenderClass* GetRender();

	private:
		~RenderClass();
		RenderClass();
	
	};


}

#endif
