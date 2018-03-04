#include "Shader.hpp"

#include <fstream>
#include <iostream>

namespace
{
	bool getFileContents(const std::string& filename, std::vector<char>& buffer)
	{
		std::ifstream file(filename.c_str(), std::ios_base::binary);
		if (file)
		{
			file.seekg(0, std::ios_base::end);
			std::streamsize size = file.tellg();
			if (size > 0)
			{
				file.seekg(0, std::ios_base::beg);
				buffer.resize(static_cast<std::size_t>(size));
				file.read(&buffer[0], size);
			}
			buffer.push_back('\0');
			return true;
		}
		else
		{
			return false;
		}
	}

	void printProgramLog(GLuint obj)
	{
		GLint infoLogLength = 0;
		GLsizei charsWritten = 0;
		GLchar *infoLog;

		glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::cout << infoLogLength << std::endl;

		if (infoLogLength > 0)
		{
			infoLog = (char *)malloc(infoLogLength);
			glGetProgramInfoLog(obj, infoLogLength, &charsWritten, infoLog);
			printf("%s\n", infoLog);
			free(infoLog);
		}
	}
}

bool Shader::loadFromFile(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
	// Read the vertex shader file
	std::vector<char> vertexShader;
	if (!getFileContents(vertexShaderFilename, vertexShader))
	{
		std::cout << "Failed to open vertex shader file: " << vertexShaderFilename << "\"" << std::endl;
		return false;
	}

	// Read the fragment shader file
	std::vector<char> fragmentShader;
	if (!getFileContents(fragmentShaderFilename, fragmentShader))
	{
		std::cout << "Failed to open fragment shader file: " << fragmentShaderFilename << "\"" << std::endl;
		return false;
	}

	return load(vertexShader, fragmentShader);
}

bool Shader::loadFromSource(const std::string& vertexShader, const std::string& fragmentShader)
{
	return load(std::vector<char>(vertexShader.begin(), vertexShader.end()), std::vector<char>(fragmentShader.begin(), fragmentShader.end()));
}

bool Shader::load(const std::vector<char>& vertexShader, const std::vector<char>& fragmentShader)
{
	GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint vert = glCreateShader(GL_VERTEX_SHADER);

	if (frag == 0)
	{
		std::cout << "Error creating shader type: Fragment" << std::endl;

		return false;
	}
	if (vert == 0)
	{
		std::cout << "Error creating shader type: Vertex" << std::endl;

		return false;
	}

	m_name = glCreateProgram();

	if (!compile(fragmentShader, frag) || !compile(vertexShader, vert))
	{
		return false;
	}

	glLinkProgram(m_name);

	//Check for errors
	GLint programSuccess = GL_TRUE;
	glGetProgramiv(m_name, GL_LINK_STATUS, &programSuccess);

	if (programSuccess != GL_TRUE)
	{
		std::cout << "Error linking program: " << m_name << std::endl;
		printProgramLog(m_name);
		glDeleteProgram(m_name);
		glDetachShader(m_name, frag);
		glDetachShader(m_name, vert);
		m_name = 0;
		return false;
	}

	glDetachShader(m_name, frag);
	glDetachShader(m_name, vert);
	glDeleteShader(frag);
	glDeleteShader(vert);

	return true;
}

bool Shader::compile(const std::vector<char>& buffer, GLuint shader)
{
	const GLchar* p[1];
	p[0] = &buffer[0];
	GLint lengths[1];
	lengths[0] = buffer.size();

	glShaderSource(shader, 1, p, lengths);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		GLchar InfoLog[1024];

		glGetShaderInfoLog(shader, 1024, NULL, InfoLog);

		std::cout << InfoLog << std::endl;

		return false;
	}

	glAttachShader(m_name, shader);

	return true;
}