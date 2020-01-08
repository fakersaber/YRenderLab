#ifndef _YRENDER_BASIC_MESH_CUBE_H_
#define _YRENDER_BASIC_MESH_CUBE_H_


#include <Public/YGM/Vector2.hpp>
#include <Public/YGM/Vector3.hpp>
#include <vector>



	class Cube {
	public:
		Cube();
		const std::vector<Vector3>& GetPositions() const { return position; }
		const std::vector<Vector3>& GetNormals() const { return normal; }
		const std::vector<Vector2>& GetTexcoords() const { return texcoords; }
		const std::vector<unsigned int>& GetIndice() const { return indice; }


	protected:
		std::vector<unsigned int> indice;
		std::vector<Vector3> position;
		std::vector<Vector3> normal;
		std::vector<Vector2> texcoords;

		static const float cubeData[192];
	};




#endif