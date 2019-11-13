#ifndef _YRENDER_SCENE_ASSIMP_LOADER_H_
#define _YRENDER_SCENE_ASSIMP_LOADER_H_

#include <Public/3rdPart/assimp/Importer.hpp>
#include <Public/3rdPart/assimp/scene.h>
#include <Public/3rdPart/assimp/postprocess.h>

#include <unordered_map>

namespace YRender{
	class YObject;
	class Image;
	namespace AssimpLoader {
		const std::shared_ptr<YObject> Load(const std::string& path);

		const std::shared_ptr<YObject> LoadNodes(
			std::unordered_map<std::string, std::shared_ptr<Image>>& image_table, 
			const std::string& dir, 
			aiNode* node, 
			const aiScene* scene);

		void LoadMesh(
			std::unordered_map<std::string, std::shared_ptr<Image>>& image_table,
			const std::string& dir,
			aiMesh* mesh,
			const aiScene* scene,
			std::shared_ptr<YObject> meshobj);

		std::shared_ptr<Image> LoadTexture(
			std::unordered_map<std::string, std::shared_ptr<Image>>& image_table,
			const std::string& dir,
			aiMaterial* material,
			aiTextureType type
		);
	}
}

#endif
