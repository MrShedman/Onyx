#include "VBO.hpp"

#include <cassert>
#include <utility>

VBO::VBO(GLenum target, GLenum usage) : m_target(target), m_usage(usage)
{
	glGenBuffers(1, &m_name);
}

VBO::~VBO()
{
	glDeleteBuffers(1, &m_name);
}


VBO::VBO(VBO&& other) :
	m_name(0u),
	m_target(0u),
	m_usage(0u)
{
	std::swap(m_name, other.m_name);
	std::swap(m_target, other.m_target);
	std::swap(m_usage, other.m_usage);
}

VBO& VBO::operator=(VBO&& other)
{
	if (this != &other)
	{
		m_name = 0u;
		m_target = 0u;
		m_usage = 0u;

		std::swap(m_name, other.m_name);
		std::swap(m_target, other.m_target);
		std::swap(m_usage, other.m_usage);
	}

	return *this;
}

GLuint VBO::name() const
{
	return m_name;
}

void VBO::bind() const
{
	check_gl_error(glBindBuffer(m_target, m_name));
}

void VBO::unbind() const
{
	check_gl_error(glBindBuffer(m_target, NULL));
}

void VBO::data(GLsizeiptr data_size, const GLvoid* data_ptr)
{
	assert(data_size >= 0);

	bind();

	check_gl_error(glBufferData(m_target, data_size, data_ptr, m_usage));

	unbind();
}