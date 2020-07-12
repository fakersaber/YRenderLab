#ifndef _YRENDER_WINDOWINTERFACE_H_
#define _YRENDER_WINDOWINTERFACE_H_

#include <memory>

class RHI;
class RHIViewport;

class IWindowInterface {
public:
	virtual ~IWindowInterface() {}
	virtual void Init(const int width, const int height, const std::shared_ptr<RHI>& RHIResource) = 0;
	virtual void Shutdown()= 0;
	virtual void Tick() = 0;

	const RHIViewport* GetViewportRHI() const { return ViewportRHI; }

protected:
	RHIViewport* ViewportRHI;
};


#endif
