#ifndef _YRENDER_YCORE_H_
#define _YRENDER_YCORE_H_



class YApplication {
public:
	YApplication() {};
	virtual ~YApplication() {};
	virtual void Init(const int width, const int height) = 0;
	virtual void Shutdown() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Tick() = 0;

protected:


	//Ubpa::UECS::World world;
	//Ubpa::UECS::Entity cam{ Ubpa::UECS::Entity::Invalid() };
};



#endif