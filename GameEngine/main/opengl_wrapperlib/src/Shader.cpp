#include "..\include\Shader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader()
: m_shaderId(0)
{
}

bool Shader::loadFromString(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
	const char* vertexSourceStr = vertexSource.c_str();
	const char* fragmentSourceStr = fragmentSource.c_str();

	// Vertex Shader
	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexSourceStr, nullptr);
	glCompileShader(vertex);

	// Print compile errors if any
	int success;
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

		return false;
	};

	// Similiar for Fragment Shader
	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentSourceStr, nullptr);
	glCompileShader(fragment);

	// Print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

		return false;
	};

	// Shader Program
	m_shaderId = glCreateProgram();
	glAttachShader(m_shaderId, vertex);
	glAttachShader(m_shaderId, fragment);
	glLinkProgram(m_shaderId);

	// Print linking errors if any
	glGetProgramiv(m_shaderId, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(m_shaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

		return false;
	}

	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

bool Shader::loadFromFile(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
	std::ifstream vertexShaderFile(vertexShaderFilename, std::ifstream::in);
	if (vertexShaderFile.is_open())
	{
		std::cout << "ERROR::SHADER::VERTEX::FILE_NOT_SUCCESFULLY_OPENED" << std::endl;

		return false;
	}

	std::ifstream fragmentShaderFile(fragmentShaderFilename, std::ifstream::in);
	if (fragmentShaderFile.is_open())
	{
		std::cout << "ERROR::SHADER::FRAGMENT::FILE_NOT_SUCCESFULLY_OPENED" << std::endl;

		return false;
	}

	std::stringstream vertexShaderStream;
	std::stringstream fragmentShaderStream;

	// Read file's buffer contents into streams
	vertexShaderStream << vertexShaderFile.rdbuf();
	fragmentShaderStream << fragmentShaderFile.rdbuf();

	// Close file handlers
	vertexShaderFile.close();
	fragmentShaderFile.close();

	return loadFromString(vertexShaderStream.str(), fragmentShaderStream.str());
}

void Shader::use()
{
	glUseProgram(m_shaderId);
}

void Shader::setUniform(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_shaderId, name.c_str()), static_cast<int>(value));
}

void Shader::setUniform(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_shaderId, name.c_str()), value);
}

void Shader::setUniform(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(m_shaderId, name.c_str()), value);
}
