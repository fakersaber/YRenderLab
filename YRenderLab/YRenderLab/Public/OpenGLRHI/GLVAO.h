#ifndef _YRENDER_OPENGLRHI_GLVAO_H
#define _YRENDER_OPENGLRHI_GLVAO_H

#include <vector>


class GLShader;

//现在的shader,VAO,VBO,EBO,FBO,Texture等所有资源都没有释放，后续需要资源管理
class VAO {
public:
	struct VBO_DataPatch {
		const float* data;
		uint32_t dataSize;
		uint32_t attrLen;
	};
	VAO();

	VAO(float const* data, uint32_t dataSize, const std::vector<uint32_t>& attrLen);

	VAO(float const* data, uint32_t dataSize, const std::vector<uint32_t>& attrLen, uint32_t const* indexArray, uint32_t indexSize);

	VAO(const std::vector<VBO_DataPatch>& vec_VBO_DataPatch, const std::vector<uint32_t>& divisors = std::vector<uint32_t>());

	VAO(const std::vector<VBO_DataPatch>& vec_VBO_DataPatch, uint32_t const* index, uint32_t indexSize, const std::vector<uint32_t>& divisors = std::vector<uint32_t>());
	//~VAO();

	//uint32_t GetID() const;
	//uint32_t GetAttrNum() const;
	//bool IsValid() const;
	bool Use() const;
	bool Draw(const GLShader& shader) const;
private:
	bool GenBindEBO(uint32_t const* index, uint32_t indexSize);
	//------------
	//uint32_t attrNum;
	uint32_t VAO_ID;
	uint32_t VBO_ID;
	uint32_t EBO_ID;
	uint32_t VertexNum;
	bool hasIndex;
	bool isValid;

	//public:
	//	static const VAO inValid;
};



#endif