#pragma once

#include <string_view>

namespace opengl_wrapper
{
	using ShaderProgram_t = int;

	class OpenGlWrapper
	{
	public:
		OpenGlWrapper(const unsigned int screenWidth = 800, const unsigned int screenHeight = 600, const std::string_view windowName = "Default Window Name");
		~OpenGlWrapper();

		ShaderProgram_t createProgram(const char* vertexShaderSource, const char* fragmentShaderSource) const;
	private:
		GLFWwindow* m_mainWindow;
	};
}