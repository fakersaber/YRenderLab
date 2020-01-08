#include <Public/Basic/Mesh/TriMesh.h>
#include <Public/Basic/Mesh/Cube.h>

	//Create static Cube
	std::shared_ptr<TriMesh> TriMesh::OriginCube = GenCube();


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


	TriMesh::TriMesh(
		const int indiceNum,
		const int vertexNum,
		const unsigned int* indice, 
		const Vector3* positions, 
		const Vector3* normals, 
		const Vector2* texcoords, 
		const Vector3* tangents)
	{
		if (!indice || !positions || !normals || !texcoords) {
			printf("ERROR: TriMesh is invalid.\n");
			return;
		}
	}


	void TriMesh::InitAfterNew(){
		//auto triMesh = shared_this<TriMesh>();
		//for(auto )
	}
	std::shared_ptr<TriMesh> TriMesh::GenCube()
	{
		Cube cube;
		auto cubeMesh = New<TriMesh>(cube.GetIndice(),cube.GetPositions(),cube.GetNormals(), cube.GetTexcoords());
		return cubeMesh;
	}