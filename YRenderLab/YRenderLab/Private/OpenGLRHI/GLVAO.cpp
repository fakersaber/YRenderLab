#include <Public/OpenGLRHI/GLVAO.h>
#include <Public/OpenGLRHI/GLShader.h>
#include <glad/glad.h>

namespace YRender {

	VAO::VAO() :isValid(false) { }


	VAO::VAO(float const* data, uint32_t dataSize, const std::vector<uint32_t>& attrLen) {
		if (data == NULL || dataSize == 0 || attrLen.size() == 0) {
			isValid = false;
			VAO_ID = 0;
			return;
		}

		glGenVertexArrays(1, &VAO_ID);
		glBindVertexArray(VAO_ID);

		glGenBuffers(1, &VBO_ID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
		uint32_t strideLen = 0;
		for (auto len : attrLen)
			strideLen += len;
		for (uint32_t i = 0, offset = 0; i < attrLen.size(); ++i) {
			glVertexAttribPointer(i, attrLen[i], GL_FLOAT, GL_FALSE, strideLen * sizeof(float), reinterpret_cast<void*>(offset * sizeof(float)));
			glEnableVertexAttribArray(i);
			offset += attrLen[i];
		}

		//不存在Index array时的vertex num
		VertexNum = dataSize / (strideLen * sizeof(float));
		isValid = true;
		hasIndex = false;
	}

	VAO::VAO(float const* data, uint32_t dataSize, const std::vector<uint32_t>& attrLen, uint32_t const* indexArray, uint32_t indexSize)
		: VAO(data, dataSize, attrLen) {
		if (isValid) {
			VertexNum = indexSize / sizeof(uint32_t);
			hasIndex = true;
			isValid = GenBindEBO(indexArray, indexSize);
		}
	}



	VAO::VAO(const std::vector<VBO_DataPatch>& vec_VBO_DataPatch, const std::vector<uint32_t>& divisors) {
		if (vec_VBO_DataPatch.size() == 0
			|| (divisors.size() > 0 && vec_VBO_DataPatch.size() != divisors.size())) {
			isValid = false;
			return;
		}

		glGenVertexArrays(1, &VAO_ID);
		glBindVertexArray(VAO_ID);

		for (uint32_t i = 0; i < vec_VBO_DataPatch.size(); i++) {
			auto & dataPatch = vec_VBO_DataPatch[i];
			uint32_t VBO;
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, dataPatch.dataSize, dataPatch.data, GL_STATIC_DRAW);
			glVertexAttribPointer(i, dataPatch.attrLen, GL_FLOAT, GL_FALSE, dataPatch.attrLen * sizeof(float), (void*)(0 * sizeof(float)));
			glEnableVertexAttribArray(i);
			if (divisors.size() != 0 && divisors[i] > 0)
				glVertexAttribDivisor(i, divisors[i]);
		}
		VertexNum = vec_VBO_DataPatch[0].dataSize / (sizeof(float) * vec_VBO_DataPatch[0].attrLen);
		hasIndex = false;
		isValid = true;
	}

	VAO::VAO(const std::vector<VBO_DataPatch>& vec_VBO_DataPatch, uint32_t const * index, uint32_t indexSize, const std::vector<uint32_t> & divisors)
		: VAO(vec_VBO_DataPatch, divisors) {
		if (isValid) {
			VertexNum = indexSize / sizeof(uint32_t);
			hasIndex = true;
			isValid = GenBindEBO(index, indexSize);
		}
	}

	bool VAO::GenBindEBO(uint32_t const* indexArray, uint32_t indexSize) {
		glGenBuffers(1, &EBO_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indexArray, GL_STATIC_DRAW);
		return true;
	}


	VAO::~VAO() {
		glDeleteVertexArrays(1, &VAO_ID);
		glDeleteBuffers(1, &VBO_ID);
		if (hasIndex)
			glDeleteBuffers(1, &EBO_ID);
	}

	bool VAO::Use() const {
		if (!isValid) {
			printf("ERROR::VAO::Use:\n"
				"\t""use a invalid VAO\n");
			return false;
		}

		glBindVertexArray(VAO_ID);
		return true;
	}

	bool VAO::Draw(const GLShader& shader) const {
		if (!Use() || !shader.Use())
			return false;
		if (hasIndex)
			glDrawElements(GL_TRIANGLES, VertexNum, GL_UNSIGNED_INT, NULL);
		else
			glDrawArrays(GL_TRIANGLES, 0, VertexNum);

		return true;
	}
}