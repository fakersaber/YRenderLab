#include <Public/Basic/Mesh/TriMesh.h>

namespace YRender {
	TriMesh::TriMesh(
		const std::vector<unsigned int>& indice, 
		const std::vector<Vector3>& position, 
		const std::vector<Vector3>& normal, 
		const std::vector<Vector2>& texcoords, 
		const std::vector<Vector3>& tangents)
		:
		indice(indice),
		position(position),
		normal(normal),
		texcoords(texcoords),
		tangents(tangents)
	{
		if (indice.size() <= 0 
			|| indice.size() % 3 != 0
			|| position.size() <=0
			|| normal.size() !=position.size()
			|| texcoords.size() !=position.size()
			|| (tangents.size()!=0 && tangents.size()!=position.size())
			) 
		{
			printf("ERROR: TriMesh is invalid.\n");
			return;
		}

		//for (unsigned int i = 0; i < indice.size(); i += 3) {
		//	triangles.push_back(Triangle::New(indice[i], indice[i + 1], indice[i + 2]));
		//}

		//if(tangents.empty())
		//	GenTangents();
	}
}