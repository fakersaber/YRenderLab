#ifndef _YRENDER_WINDOWTOOL_H
#define _YRENDER_WINDOWTOOL_H

#include <Public/SoftRHI/RenderClass.h>


class RenderWindow {
public:
	RenderWindow(const RenderWindow& rhs) = delete;
	RenderWindow& operator=(const RenderWindow& rhs) = delete;

public:
	virtual bool Initial(const int width, const int height) = 0;
	virtual void Run() = 0;

protected:
	RenderWindow();
	virtual ~RenderWindow();

};



#endif
