#ifndef _YRENDER_YCORE_H_
#define _YRENDER_YCORE_H_

#include <cassert>

class RHI;
class IPipeline;
class RenderScene;

class YApplication {
public:
	YApplication() {};

	virtual ~YApplication() {
		delete World;
		delete RenderRHI;
		delete RenderPipeline;
	};

	virtual void Init(const int width, const int height) = 0;
	virtual void Shutdown() = 0;
	virtual void Tick() = 0;

protected:
	void Update() {
		assert(false);
	};


	void Render() {
		assert(false);
	}

	//[Resource management]
	RHI* RenderRHI;
	IPipeline* RenderPipeline;
	RenderScene* World;
};



#endif