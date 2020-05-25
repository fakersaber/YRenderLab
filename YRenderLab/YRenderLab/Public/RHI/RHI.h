#ifndef _YRENDER_RHI_H_
#define _YRENDER_RHI_H_


class RHI {
public:
	virtual ~RHI() {};
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
};



#endif