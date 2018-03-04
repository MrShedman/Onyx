#pragma once

#include "Transform.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "Shader.hpp"

#include <memory>
#include <map>
#include <vector>

class Sphere : public Transform
{
public:

	Sphere() {}

	Sphere(const unsigned int depth, const float radius)
	{
		create(depth, radius);
	}

	Sphere(Sphere&& other) :
		Transform(std::move(other)),
		m_mesh(std::move(other.m_mesh))
	{}

	Sphere& operator=(Sphere&& other)
	{
		if (this != &other)
		{
			Transform::operator=(std::move(other));
			m_mesh = std::move(other.m_mesh);
		}

		return *this;
	}

	void create(const unsigned int depth, const float radius)
	{
		m_mesh = Mesh::create();

		const float X = 0.525731112119133606f;
		const float Z = 0.850650808352039932f;

		const Vector3f vdata[12] = {
			{ -X, 0.0, Z },{ X, 0.0, Z },{ -X, 0.0, -Z },{ X, 0.0, -Z },
			{ 0.0, Z, X },{ 0.0, Z, -X },{ 0.0, -Z, X },{ 0.0, -Z, -X },
			{ Z, X, 0.0 },{ -Z, X, 0.0 },{ Z, -X, 0.0 },{ -Z, -X, 0.0 }
		};

		GLuint tindices[20][3] = {
			{ 0, 4, 1 },{ 0, 9, 4 },{ 9, 5, 4 },{ 4, 5, 8 },{ 4, 8, 1 },
			{ 8, 10, 1 },{ 8, 3, 10 },{ 5, 3, 8 },{ 5, 2, 3 },{ 2, 7, 3 },
			{ 7, 10, 3 },{ 7, 6, 10 },{ 7, 11, 6 },{ 11, 0, 6 },{ 0, 1, 6 },
			{ 6, 1, 10 },{ 9, 0, 11 },{ 9, 11, 2 },{ 9, 2, 5 },{ 7, 2, 11 }
		};

		for (int i = 0; i < 20; i++)
		{
			subdivide(vdata[tindices[i][0]], vdata[tindices[i][1]], vdata[tindices[i][2]], depth);
		}

		m_mesh->complete();

		m_mesh->bind();
		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
		// Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		m_mesh->unbind();

		//m_mesh->updateNormals();

		setScale(radius, radius, radius);
	}

	void render(Shader& shader, Camera& camera)
	{
		// shader parameters
		shader.setUniform("objectColour", 1.0f, 0.4f, 0.4f);
		shader.setUniform("lightColour", 1.0f, 1.0f, 1.0f);
		shader.setUniform("lightPos", 0.5f, 20.0f, 0.5f);
		shader.setUniform("viewPos", camera.getPosition());
		shader.setUniform("fade", 0.0f);

		// Pass the matrices to the shader
		shader.setUniform("modelViewMatrix", getTransform());
		shader.setUniform("projectionMatrix", camera.getProjection());

		shader.bind();

		// Draw mesh
		m_mesh->draw(true);
	}

	Mesh::Ptr getMesh() const
	{
		return m_mesh;
	}

private:

	void subdivide(const Vector3f &v1, const Vector3f &v2, const Vector3f &v3, const unsigned int depth)
	{
		if (depth == 0)
		{
			m_mesh->addVertex(Vertex(v1, v1));
			m_mesh->addIndex(m_mesh->getSize() - 1);

			m_mesh->addVertex(Vertex(v2, v2));
			m_mesh->addIndex(m_mesh->getSize() - 1);

			m_mesh->addVertex(Vertex(v3, v3));
			m_mesh->addIndex(m_mesh->getSize() - 1);

			return;
		}

		const Vector3f v12 = (v1 + v2).Normalized();
		const Vector3f v23 = (v2 + v3).Normalized();
		const Vector3f v31 = (v3 + v1).Normalized();

		subdivide(v1, v12, v31, depth - 1);
		subdivide(v2, v23, v12, depth - 1);
		subdivide(v3, v31, v23, depth - 1);
		subdivide(v12, v23, v31, depth - 1);
	}

	Mesh::Ptr m_mesh;
};