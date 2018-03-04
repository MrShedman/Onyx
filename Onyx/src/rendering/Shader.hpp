#pragma once

#include "GL\glew.h"

#include <vector>
#include <string>
#include <map>
#include <iostream>

#define check_gl_error

#include "..\math\Vector.hpp"
#include "..\math\Matrix.hpp"

class Shader
{
public:

	Shader()
	{

	}

	~Shader()
	{
		glDeleteProgram(m_name);
	}

	Shader(const Shader& other) = delete;

	Shader operator = (const Shader&) = delete;

	Shader(Shader&& other) :
		m_name(0u)
	{
		std::swap(m_name, other.m_name);
	}

	Shader& operator=(Shader&& other)
	{
		if (this != &other)
		{
			m_name = 0u;

			std::swap(m_name, other.m_name);
		}

		return *this;
	}

	GLuint name() const
	{
		return m_name;
	}

	bool loadFromFile(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);

	bool loadFromSource(const std::string& vertexShader, const std::string& fragmentShader);

	void bind()
	{
		glUseProgram(m_name);
	}

	void unbind()
	{
		glUseProgram(0);
	}

	void setUniform(const std::string& name, int x)
	{
		if (m_name)
		{
			// Enable program
			bind();

			// Get parameter location and assign it new values
			GLint location = getParamLocation(name);
			if (location != -1)
			{
				check_gl_error(glUniform1i(location, x));
			}

			// Disable program
			unbind();
		}
	}

	void setUniform(const std::string& name, float x)
	{
		if (m_name)
		{
			// Enable program
			bind();

			// Get parameter location and assign it new values
			GLint location = getParamLocation(name);
			if (location != -1)
			{
				check_gl_error(glUniform1f(location, x));
			}

			// Disable program
			unbind();
		}
	}

	void setUniform(const std::string& name, float x, float y)
	{
		if (m_name)
		{
			// Enable program
			bind();

			// Get parameter location and assign it new values
			GLint location = getParamLocation(name);
			if (location != -1)
			{
				check_gl_error(glUniform2f(location, x, y));
			}

			// Disable program
			unbind();
		}
	}

	void setUniform(const std::string& name, float x, float y, float z)
	{
		if (m_name)
		{
			// Enable program
			bind();

			// Get parameter location and assign it new values
			GLint location = getParamLocation(name);
			if (location != -1)
			{
				check_gl_error(glUniform3f(location, x, y, z));
			}

			// Disable program
			unbind();
		}
	}

	void setUniform(const std::string& name, const Vector2f& v)
	{
		setUniform(name, v.x, v.y);
	}

	void setUniform(const std::string& name, const Vector3f& v)
	{
		setUniform(name, v.x, v.y, v.z);
	}

	void setUniform(const std::string& name, const Matrix4f& m)
	{
		if (m_name)
		{
			// Enable program
			bind();

			// Get parameter location and assign it new values
			GLint location = getParamLocation(name);
			if (location != -1)
			{
				check_gl_error(glUniformMatrix4fv(location, 1, GL_FALSE, m[0]));
			}

			// Disable program
			unbind();
		}
	}

	void setAttribPointer(const std::string& name, GLint size, GLsizei stride = 0, const GLvoid* data = NULL)
	{
		if (m_name)
		{
			// Enable program
			bind();

			// Get parameter location and assign it new values
			GLint location = getAttributeLocation(name);
			if (location != -1)
			{
				check_gl_error(glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, stride, data));
			}

			// Disable program
			unbind();
		}
	}

	void enableVertexAttribArray(const std::string& name)
	{
		if (m_name)
		{
			// Enable program
			bind();

			// Get parameter location and assign it new values
			GLint location = getAttributeLocation(name);
			if (location != -1)
			{
				check_gl_error(glEnableVertexAttribArray(location));
			}

			// Disable program
			unbind();
		}
	}


private:

	typedef std::map<std::string, int> AttributeTable;

	int getAttributeLocation(const std::string& name)
	{
		// Check the cache
		AttributeTable::const_iterator it = m_attributes.find(name);

		if (it != m_attributes.end())
		{
			// Already in cache, return it
			return it->second;
		}
		else
		{
			// Not in cache, request the location from OpenGL
			int location = glGetAttribLocation(m_name, name.c_str());
			m_attributes.insert(std::make_pair(name, location));

			if (location == -1)
			{
				std::cout << "Attribute \"" << name << "\" not found in shader" << std::endl;
			}

			return location;
		}
	}

	typedef std::map<std::string, int> UniformTable;

	int getParamLocation(const std::string& name)
	{
		// Check the cache
		UniformTable::const_iterator it = m_uniforms.find(name);

		if (it != m_uniforms.end())
		{
			// Already in cache, return it
			return it->second;
		}
		else
		{
			// Not in cache, request the location from OpenGL
			int location = glGetUniformLocation(m_name, name.c_str());
			m_uniforms.insert(std::make_pair(name, location));

			if (location == -1)
			{
				std::cout << "Parameter \"" << name << "\" not found in shader" << std::endl;
			}

			return location;
		}
	}

	AttributeTable m_attributes;
	UniformTable m_uniforms;

	bool load(const std::vector<char>& vertexShader, const std::vector<char>& fragmentShader);

	bool compile(const std::vector<char>& buffer, GLuint shader);

private:

	GLuint	m_name;
};