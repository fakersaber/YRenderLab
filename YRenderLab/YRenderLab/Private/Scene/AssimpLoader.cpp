#include <Public/3rdPart/assimp/pbrmaterial.h>

#include <Public/Scene/AssimpLoader.h>
#include <Public/Scene/Yobject.h>
#include <Public/Scene/TransformComponent.h>


#include <Public/Basic/StrApi.h>
#include <Public/Basic/Image/Image.h>

#include <iostream>

namespace YRender{
	namespace AssimpLoader {

		const std::shared_ptr<YObject> Load(const std::string& path)
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
			auto dir = path.substr(0, path.find_last_of('/'));
			std::unordered_map<std::string, std::shared_ptr<Image>> image_table;
			return LoadNodes(image_table, dir, scene->mRootNode, scene);
		}

		const std::shared_ptr<YObject> LoadNodes
		(
			std::unordered_map<std::string, std::shared_ptr<Image>>& image_table, 
			const std::string& dir, 
			aiNode* node, 
			const aiScene* scene
		)
		{
			auto obj = YRender::New<YObject>(node->mName.C_Str());
			YRender::New<TransformComponent>(obj);
			for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
				//每一个mesh都是一个渲染对象
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				
				auto meshObj = YRender::New<YObject>("mesh_" + std::to_string(i));
				YRender::New<TransformComponent>(meshObj);
				LoadMesh(image_table, dir, mesh, scene, meshObj);
			}
			return obj;
		}

		void LoadMesh(
			std::unordered_map<std::string, std::shared_ptr<Image>>& image_table,
			const std::string& dir,
			aiMesh* mesh,
			const aiScene* scene,
			std::shared_ptr<YObject> meshobj
		) 
		{
			
		}
	}
}
