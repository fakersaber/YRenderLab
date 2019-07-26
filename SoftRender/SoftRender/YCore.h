#ifndef _YCORE_H
#define _YCORE_H

#include "RenderClass.h"
#include "RenderWindow.h"

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