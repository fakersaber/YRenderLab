#ifndef _YRENDER_SCENE_ASSIMP_LOADER_H_
#define _YRENDER_SCENE_ASSIMP_LOADER_H_

#include <Public/3rdPart/assimp/Importer.hpp>
#include <Public/3rdPart/assimp/scene.h>

namespace YRender{
	class Yobject;

	class AssimpLoader {
		static const std::shared_ptr<Yobject> Load(const std::string& path);

	};
}

#endif