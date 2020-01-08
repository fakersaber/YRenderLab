#ifndef _YRENDER_SCENE_SCENE_H_
#define _YRENDER_SCENE_SCENE_H_

#include <Public/Basic/YHeapObject.h>
#include <Public/Basic/Camera/Camera.h>


class YObject;
class Image;


class Scene : public YHeapObject {
public:
	Scene(std::shared_ptr<YObject> root, std::shared_ptr<Camera> camera, std::shared_ptr<Image> EnviromentImg) :
		root(root),
		camera(camera),
		EnviromentImg(EnviromentImg)
	{
	}

protected:
	virtual ~Scene() = default;

public:
	const std::shared_ptr<Image> GetEnviromentImg() const { return EnviromentImg; }
	const std::shared_ptr<YObject> GetRoot() const { return root; }
	const std::shared_ptr<Camera> GetCamera() const { return camera; }

private:
	std::shared_ptr<YObject> root;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Image> EnviromentImg;
};


#endif