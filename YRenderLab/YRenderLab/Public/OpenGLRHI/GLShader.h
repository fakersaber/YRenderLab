#ifndef _YRENDER_OPENGLRHI_GLSHADER_H
#define _YRENDER_OPENGLRHI_GLSHADER_H

#include <Public/Basic/BasicCore.h>
#include <Public/3rdPart/enum.h>

namespace YRender {

	BETTER_ENUM(CompileType, int, VERTEX, GEOMETRY, FRAGMENT, PROGRAM);

	class GLShader {
	public:
		GLShader();
		GLShader(const std::string& vertexPath, const std::string& fragmentPath);

		bool Use() const;

		void SetBool(const std::string &name, bool value) const;
		void SetInt(const std::string &name, int value) const;
		void SetTex(const std::string &name, int value) const {
			SetInt(name, value);
		}
		void SetFloat(const std::string &name, float value) const;
		void SetFloats(const std::string &name, uint32_t n, const float * values) const;
		void SetVec2f(const std::string &name, float value0, float value1) const;
		void SetVec2f(const std::string &name, const Val2& v) const {
			SetVec2f(name, v[0], v[1]);
		}
		void SetVec3f(const std::string &name, float value0, float value1, float value2) const;
		void SetVec3f(const std::string &name, const Val3& v) const {
			SetVec3f(name, v[0], v[1], v[2]);
		}
		void SetVec3fs(const std::string& name, uint32_t n, const float* data) const;
		void SetVec4f(const std::string& name, float value0, float value1, float value2, float value3) const;
		void SetVec4f(const std::string& name, const Val4& v) const {
			SetVec4f(name, v[0], v[1], v[2], v[3]);
		}
		void SetMat4f(const std::string &name, const float* matValue) const;
		void SetMat4f(const std::string &name, const Mat4f& mat) const {
			SetMat4f(name, mat.Data());
		}
		void SetMat4f(const std::string &name, const YGM::Transform& tsfm) const {
			SetMat4f(name, tsfm.GetMatrix());
		}

		void UniformBlockBind(const std::string& name, uint32_t bindPoint) const;

	//public:
	//	static const GLShader InValid;

	private:
		int CheckCompileErrors(uint32_t shader, CompileType type);

	private:
		uint32_t ID;
		bool valid;
	};
}



#endif