#pragma once

#include "GL\glew.h"

//#include "ErrorCheck.hpp"
#define check_gl_error

class VBO
{
public:

	VBO(GLenum target, GLenum usage);
	
	~VBO();

	VBO(const VBO& other) = delete;

	VBO operator = (const VBO&) = delete;

	VBO(VBO&& other);

	VBO& operator=(VBO&& other);

	GLuint name() const;

	void bind() const;

	void unbind() const;

	void data(GLsizeiptr data_size, const GLvoid* data_ptr);

private:

	GLenum m_target;
	GLenum m_usage;

	GLuint m_name;
};