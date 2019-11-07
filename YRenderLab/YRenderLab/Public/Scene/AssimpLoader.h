#ifndef _YRENDER_SCENE_ASSIMP_LOADER_H_
#define _YRENDER_SCENE_ASSIMP_LOADER_H_

#include <Public/3rdPart/assimp/Importer.hpp>
#include <Public/3rdPart/assimp/scene.h>
#include <Public/3rdPart/assimp/postprocess.h>

namespace YRender{
	namespace AssimpLoader {
		class Yobject;

		const std::shared_ptr<Yobject> Load(const std::string& path);
	}
}

#endif
