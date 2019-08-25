﻿#ifndef _YRENDER_YCORE_H
#define _YRENDER_YCORE_H

#include <Public/RenderClass.h>
#include <Public/RenderWindow.h>

namespace YRender {
	class YCore {
	public:
		YCore(const YCore& rhs) = delete;
		YCore& operator=(const YCore& rhs) = delete;
		static YCore* GetCore();

	public:
		bool Initial(const int width, const int height);
		void Run();


	private:
		YCore();
		~YCore();


	private:
		RenderWindow* _RenderWindow;
		RenderClass* _RenderClass;

	};
}


#endif