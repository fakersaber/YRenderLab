#ifndef _YRENDER_BASIC_MESH_TRIMESH_H_
#define _YRENDER_BASIC_MESH_TRIMESH_H_

#include <Public/Basic/YHeapObject.h>
#include <Public/YGM/Vector.hpp>
#include <vector>

namespace YRender {
	class TriMesh final : public YHeapObject {
	public:
		TriMesh(
			const std::vector<unsigned int>& indice,
			const std::vector<Vector3>& position,
			const std::vector<Vector3>& normal,
			const std::vector<Vector2>& texcoords,
			const std::vector<Vector3>& tangents = std::vector<Vector3>()
		);

		


	private:
		std::vector<unsigned int> indice;
		std::vector<Vector3> position;
		std::vector<Vector3> normal;
		std::vector<Vector2> texcoords;
		std::vector<Vector3> tangents;

		//BoundBox °ó¶¨ºÐ´ýÐø
	};
}



#endif