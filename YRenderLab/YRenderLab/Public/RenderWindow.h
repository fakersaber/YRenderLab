#ifndef _YRENDER_WINDOWTOOL_H
#define _YRENDER_WINDOWTOOL_H

#include <iostream>

#include <Public/RenderClass.h>

namespace YRender {
	class RenderWindow {
	public:
		RenderWindow(const RenderWindow& rhs) = delete;
		RenderWindow& operator=(const RenderWindow& rhs) = delete;

	public:
		virtual bool Initial(const int width, const int height) = 0;
		virtual void Run() = 0;

	public:
		RenderWindow();
		virtual ~RenderWindow();

	protected:
		RenderClass* _RenderClass;
	};


}


#endif
