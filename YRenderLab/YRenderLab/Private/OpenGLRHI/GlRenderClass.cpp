#include <Public/OpenGLRHI/GlRenderClass.h>




namespace YRender {
	GlRenderClass::GlRenderClass(GLFWwindow * window){

	}

	GlRenderClass::~GlRenderClass(){

	}

	bool GlRenderClass::Initial(const int width, const int height)
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return false;
		}
		return true;
	}

	void GlRenderClass::Tick(){

	}

	void GlRenderClass::Render(){
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		//glUseProgram(shaderProgram);
		//glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//glDrawArrays(GL_TRIANGLES, 0, 6); // set the count to 6 since we're drawing 6 vertices now (2 triangles); not 3!
		// glBindVertexArray(0); // no need to unbind it every time 

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
	}
}


