#ifndef _YRENDER_YCORE_H_
#define _YRENDER_YCORE_H_

#include <cassert>



class YApplication {
public:
	YApplication() {};
	virtual ~YApplication() {};
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

	//Ubpa::UECS::World world;
	//Ubpa::UECS::Entity cam{ Ubpa::UECS::Entity::Invalid() };
};



#endif