#include <Public/3rdPart/assimp/pbrmaterial.h>

#include <Public/Scene/AssimpLoader.h>
#include <Public/Scene/Yobject.h>
#include <Public/Basic/StrApi.h>

#include <iostream>

namespace YRender{
	namespace AssimpLoader {

		const std::shared_ptr<Yobject> Load(const std::string & path)
		{
			bool isSupport = false;
			const std::string suffix[] = { ".FBX",".obj" };
			for(auto& temp : suffix){
				if (StrAPI::Is_suffix(path, temp)) {
					isSupport = true;
					break;
				}
			}		
			if (!isSupport)
				return nullptr;
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals);
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
				std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
				return nullptr;
			}

		}


	}
}
