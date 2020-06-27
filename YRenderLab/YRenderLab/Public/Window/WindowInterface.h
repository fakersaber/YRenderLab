#ifndef _YRENDER_WINDOWINTERFACE_H_
#define _YRENDER_WINDOWINTERFACE_H_


class IWindowInterface {
public:
	virtual ~IWindowInterface() {}
	virtual bool Initial(const int width, const int height) = 0;
	virtual void Shutdown()= 0;
	virtual void Run() = 0;


protected:

};


#endif
