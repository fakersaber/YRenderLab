#include <Public/OpenGLRHI/GLShader.h>
#include <Public/OpenGLRHI/GLAD/glad/glad.h>
namespace YRender {
	GLShader::GLShader() : valid(false){

	}

	GLShader::GLShader(const std::string &  vertexPath, const std::string & fragmentPath, const std::string & geometryPath) {
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER); // the first fragment shader that outputs the color orange
		unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER); // the second fragment shader that outputs the color yellow
		unsigned int shaderProgramOrange = glCreateProgram();
		unsigned int shaderProgramYellow = glCreateProgram(); // the second shader program
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		glShaderSource(fragmentShaderOrange, 1, &fragmentShader1Source, NULL);
		glCompileShader(fragmentShaderOrange);
		glShaderSource(fragmentShaderYellow, 1, &fragmentShader2Source, NULL);
		glCompileShader(fragmentShaderYellow);
		// link the first program object
		glAttachShader(shaderProgramOrange, vertexShader);
		glAttachShader(shaderProgramOrange, fragmentShaderOrange);
		glLinkProgram(shaderProgramOrange);
		// then link the second program object using a different fragment shader (but same vertex shader)
		// this is perfectly allowed since the inputs and outputs of both the vertex and fragment shaders are equally matched.
		glAttachShader(shaderProgramYellow, vertexShader);
		glAttachShader(shaderProgramYellow, fragmentShaderYellow);
		glLinkProgram(shaderProgramYellow);
	}
}

