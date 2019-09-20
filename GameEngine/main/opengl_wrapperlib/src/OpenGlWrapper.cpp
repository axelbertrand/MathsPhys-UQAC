#include "..\include\OpenGlWrapper.hpp"

#include <cassert>
#include <iostream>

namespace opengl_wrapper
{
	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}

	OpenGlWrapper::OpenGlWrapper(const unsigned int screenWidth, const unsigned int screenHeight, const std::string_view windowName)
	{
		// glfw: initialize and configure
		// ------------------------------
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif


		// glfw window creation
		// --------------------
		m_mainWindow = glfwCreateWindow(screenWidth, screenHeight, windowName.data(), NULL, NULL);
		assert(m_mainWindow != nullptr);

		glfwMakeContextCurrent(m_mainWindow);
		glfwSetFramebufferSizeCallback(m_mainWindow, framebuffer_size_callback);

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		bool success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		assert(success == true);
	}

	OpenGlWrapper::~OpenGlWrapper()
	{
		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwTerminate();
	}

	ShaderProgram_t OpenGlWrapper::createProgram(const char* vertexShaderSource, const char* fragmentShaderSource) const
	{
		// build and compile our shader program
		// ------------------------------------
		// vertex shader
		int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		// check for shader compile errors
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// fragment shader
		int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		// check for shader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// link shaders
		ShaderProgram_t shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		// check for linking errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return shaderProgram;
	}

	GLFWwindow* OpenGlWrapper::getMainWindow() const
	{
		return m_mainWindow;
	}

	bool OpenGlWrapper::windowShouldClose(GLFWwindow * const window) const
	{
		return glfwWindowShouldClose(window);
	}

	void OpenGlWrapper::clearCurrentWindow(float red, float green, float blue, float opacity) const
	{
		glClearColor(red, green, blue, opacity);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGlWrapper::useShadersProgram(ShaderProgram_t shadersProgram) const
	{
		glUseProgram(shadersProgram);
	}

	std::tuple<unsigned int, unsigned int> OpenGlWrapper::getWindowSize(GLFWwindow* const window) const
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		return { width, height };
	}

	void OpenGlWrapper::draw(GLenum shape, unsigned int count) const
	{
		glDrawElements(shape, count, GL_UNSIGNED_INT, 0);
	}

	void OpenGlWrapper::cleanAndDeleteVAO(std::tuple<unsigned int, unsigned int, unsigned int> buffers) const
	{
		glDeleteVertexArrays(1, &std::get<0>(buffers));
		glDeleteBuffers(1, &std::get<1>(buffers));
		glDeleteBuffers(1, &std::get<2>(buffers));
	}

	// create buffers to contains graphical data
	std::tuple<unsigned int, unsigned int, unsigned int> OpenGlWrapper::createAndBindVAO(const std::vector<float>& verticesBuffer,
		const std::vector<unsigned int>& indicesBuffer) const
	{
		const float * vertices = verticesBuffer.data();
		const unsigned int* indices = indicesBuffer.data();

		unsigned int VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, verticesBuffer.size()*sizeof(float), vertices, GL_STREAM_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer.size()*sizeof(unsigned int), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return { VAO, VBO, EBO };
	}

	void OpenGlWrapper::swapBuffers(GLFWwindow* const window) const
	{
		glfwSwapBuffers(window);
	}

	void OpenGlWrapper::pollEvent() const
	{
		glfwPollEvents();
	}

	void OpenGlWrapper::setKeyboardCallback(GLFWwindow* window, GLFWkeyfun callbackFunction) const
	{
		glfwSetKeyCallback(window, callbackFunction);
	}
}