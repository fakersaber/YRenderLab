#include <Public/OpenGLRHI/GLShader.h>
#include <Public/OpenGLRHI/GLAD/glad/glad.h>

#include <Public/Basic/File/File.h>
namespace YRender {
	GLShader::GLShader() : valid(false){

	}

	GLShader::GLShader(const std::string& vertexPath, const std::string& fragmentPath) {
		std::string VertexShader = File::ReadFile(vertexPath);
		std::string FragmentShader = File::ReadFile(fragmentPath);
		if (VertexShader.empty() || FragmentShader.empty()) {
			std::cout << VertexShader << " or " << FragmentShader << "Read failed" <<std::endl;
			valid = false;
			return;
		}
		const char* vertexSource = VertexShader.c_str();
		const char* fragmentSource = FragmentShader.c_str();	
		uint32_t glvs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(glvs, 1, &vertexSource, NULL);
		glCompileShader(glvs);
		if (!CheckCompileErrors(glvs, CompileType::VERTEX)) {
			std::cout << vertexPath << " compiles error\n";
			valid = false;
			return;
		}
		uint32_t glfs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(glfs, 1, &fragmentSource, NULL);
		glCompileShader(glfs);
		if (!CheckCompileErrors(glvs, CompileType::FRAGMENT)) {
			std::cout << vertexPath << " compiles error\n";
			valid = false;
			return;
		}
		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, glvs);
		glAttachShader(ID, glfs);
		glLinkProgram(ID);
		if (!CheckCompileErrors(ID, CompileType::PROGRAM)) {
			std::cout << vertexPath << " and " << fragmentPath << " link failed.\n";
			valid = false;
			return;
		}
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(glvs);
		glDeleteShader(glfs);
		valid = true;
	}

	bool GLShader::Use() const
	{
		if (!valid) {
			printf("ERROR::Shader::Use:\n"
				"\t""use an invalid shader\n");
			return false;
		}

		glUseProgram(ID);
		return true;
	}
	void GLShader::SetBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void GLShader::SetInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void GLShader::SetFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void GLShader::SetFloats(const std::string& name, uint32_t n, const float* values) const {
		glUniform1fv(glGetUniformLocation(ID, name.c_str()), n, values);
	}


	void GLShader::SetVec2f(const std::string& name, float value0, float value1) const {
		glUniform2f(glGetUniformLocation(ID, name.c_str()), value0, value1);
	}

	void GLShader::SetVec3f(const std::string& name, float value0, float value1, float value2) const {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), value0, value1, value2);
	}

	void GLShader::SetVec3fs(const std::string& name, uint32_t n, const float* data) const {
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 4, data);
	}

	void GLShader::SetVec4f(const std::string& name, float value0, float value1, float value2, float value3) const {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), value0, value1, value2, value3);
	}

	void GLShader::SetMat4f(const std::string& name, const float* matValue) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, matValue);
	}
	int GLShader::CheckCompileErrors(uint32_t shader, CompileType type) {
		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		char infoLog[1024];

		switch (type)
		{
		case CompileType::VERTEX:
		case CompileType::GEOMETRY:
		case CompileType::FRAGMENT: {
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type._to_string() << std::endl
					<< infoLog
					<< "\n -- --------------------------------------------------- -- " << std::endl;
			}
			break;
		}
		case CompileType::PROGRAM: {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << std::endl
					<< infoLog
					<< "\n -- --------------------------------------------------- -- " << std::endl;
			}
			break;
		}
		default: {
			printf("ERROR::Shader::CheckCompileErrors:\n"
				"\t""not support CompileType\n");
			break;
		}
		}

		return success;
	}
}


