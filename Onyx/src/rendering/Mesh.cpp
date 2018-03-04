
#include "Mesh.hpp"

Mesh::Mesh(size_t size)
	:
	m_verticesBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW),
	m_indicesBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW),
	m_mode(GL_TRIANGLES)
{
	resize(size);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vao);
}

Mesh::Ptr Mesh::create(size_t size)
{
	Mesh::Ptr mesh(new Mesh(size));
	return mesh;
}

void Mesh::resize(size_t size)
{
	m_vertices.resize(size);
}

void Mesh::clear()
{
	m_vertices.clear();
	m_indices.clear();
}

void Mesh::addVertex(const Vertex& vertex)
{
	m_vertices.push_back(vertex);
}

void Mesh::addIndex(GLuint index)
{
	m_indices.push_back(index);
}

void Mesh::complete()
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	m_verticesBuffer.data(m_vertices.size() * sizeof(Vertex), m_vertices.data());

	m_indicesBuffer.data(m_indices.size() * sizeof(GLuint), m_indices.data());

	// keep these bound so only need to bind vao in future
	m_verticesBuffer.bind();
	m_indicesBuffer.bind();

	glBindVertexArray(0);
}

void Mesh::addVertices(const std::vector<Vertex>& vertices)
{
	m_vertices = vertices;
}

void Mesh::addIndices(const std::vector<GLuint>& indices)
{
	m_indices = indices;
}

size_t Mesh::getSize() const
{
	return m_vertices.size();
}

std::vector<Vertex>::const_pointer Mesh::getData() const
{
	return m_vertices.data();
}

bool Mesh::isEmpty() const
{
	return m_vertices.empty();
}

void Mesh::setPrimitiveType(GLenum mode)
{
	m_mode = mode;
}

float Mesh::getVolume(const Matrix4f& transform) const
{
	float volume = 0.0f;

	for (int i = 0; i < getSize() / 3; i++)
	{
		Triangle tri = getTriangle(i);

		tri.vert0.position = transform.transformPoint(tri.vert0.position);
		tri.vert1.position = transform.transformPoint(tri.vert1.position);
		tri.vert2.position = transform.transformPoint(tri.vert2.position);

		volume += tri.vert0.position.Dot(tri.vert1.position.Cross(tri.vert2.position)) / 6.0f;
	}

	return std::abs(volume);
}

AABBf Mesh::getLocalBounds() const
{
	AABBf meshBounds;

	for (auto&v : m_vertices)
	{
		meshBounds.include(v.position);
	}

	return meshBounds;
}

AABBf Mesh::getGlobalBounds(const Matrix4f& transform) const
{
	AABBf meshBounds;

	for (auto&v : m_vertices)
	{
		meshBounds.include(transform.transformPoint(v.position));
	}

	return meshBounds;
}

Triangle Mesh::getTriangle(std::size_t index) const
{
	GLuint i0 = m_indices[index * 3 + 0];
	GLuint i1 = m_indices[index * 3 + 1];
	GLuint i2 = m_indices[index * 3 + 2];

	return Triangle({ m_vertices[i0], m_vertices[i1], m_vertices[i2] });
}

void Mesh::updateNormals()
{
	std::vector<Vector3f> normals(m_vertices.size());

	for (unsigned int i = 0; i < normals.size(); i++)
	{
		normals[i] = Vector3f(0.0f, 0.0f, 0.0f);
	}

	for (unsigned int i = 0; i < m_indices.size(); i += 3)
	{
		GLuint i0 = m_indices[i + 0];
		GLuint i1 = m_indices[i + 1];
		GLuint i2 = m_indices[i + 2];

		Vector3f v1 = m_vertices[i1].position - m_vertices[i0].position;
		Vector3f v2 = m_vertices[i2].position - m_vertices[i0].position;

		Vector3f normal = v1.Cross(v2).Normalized();

		normals[i0] += normal;
		normals[i1] += normal;
		normals[i2] += normal;
	}

	for (unsigned int i = 0; i < normals.size(); i++)
		m_vertices[i].normal = normals[i].Normalized();

	m_verticesBuffer.data(m_vertices.size() * sizeof(Vertex), m_vertices.data());
}

void Mesh::bind() const
{
	glBindVertexArray(m_vao);
}

void Mesh::unbind() const
{
	glBindVertexArray(0);
}

void Mesh::draw(bool wireframe) const
{
	bind();

	if (wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	glDrawElements(m_mode, m_indices.size(), GL_UNSIGNED_INT, 0);

	if (wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	unbind();
}