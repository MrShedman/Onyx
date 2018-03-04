#pragma once

#include <vector>
#include <string>
#include <cassert>
#include <memory>

#include "buffers\VBO.hpp"

#include "math\Vector.hpp"
#include "math\AABB.hpp"
#include "math\Matrix.hpp"

#include "Vertex.hpp"
#include "Triangle.hpp"

class Mesh
{
public:

	typedef std::shared_ptr<Mesh> Ptr;

private:

	Mesh(size_t size = 0);

public:

	~Mesh();

	static Ptr create(size_t size = 0);

	typedef std::vector<Vertex>::iterator			iterator;
	typedef std::vector<Vertex>::const_iterator		const_iterator;

	iterator begin()
	{
		return m_vertices.begin();
	}

	const_iterator begin() const
	{
		return m_vertices.begin();
	}

	iterator end()
	{
		return m_vertices.end();
	}

	const_iterator end() const
	{
		return m_vertices.end();
	}

	inline const Vertex& operator[](int index) const
	{
		return m_vertices[index];
	}

	inline Vertex& operator[](int index)
	{
		return m_vertices[index];
	}

	void resize(size_t size);

	void clear();

	void addVertex(const Vertex& vertex);

	void addIndex(GLuint index);

	void complete();

	void addVertices(const std::vector<Vertex>& vertices);

	void addIndices(const std::vector<GLuint>& indices);

	size_t getSize() const;

	std::vector<Vertex>::const_pointer getData() const;

	bool isEmpty() const;

	void setPrimitiveType(GLenum mode);

	float getVolume(const Matrix4f& transform) const;

	AABBf getLocalBounds() const;

	AABBf getGlobalBounds(const Matrix4f& transform) const;

	Triangle getTriangle(std::size_t index) const;

	void updateNormals();

	void bind() const;

	void unbind() const;

	void draw(bool wireframe = false) const;

private:

	GLuint m_vao;
	
	GLenum m_mode;

	VBO m_verticesBuffer;
	VBO m_indicesBuffer;

	std::vector<GLuint> m_indices;
	std::vector<Vertex> m_vertices;
};