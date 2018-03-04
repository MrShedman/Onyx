#pragma once

#include "Transform.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "Shader.hpp"

#include "math\Rect.hpp"

#include <memory>

class Ground : public Transform
{
public:

	void create()
	{
		m_mesh = Mesh::create();
		m_mesh->setPrimitiveType(GL_LINES);

		m_bounds = RectF(10.0f, -10.0f, -10.0f, 10.0f);

		int id = 0;

		for (float ix = m_bounds.left; ix < m_bounds.right; ix += interval)
		{
			append(Vector2f(ix, m_bounds.bottom));
			append(Vector2f(ix, m_bounds.top));
		}

		append(Vector2f(m_bounds.right, m_bounds.bottom));
		append(Vector2f(m_bounds.right, m_bounds.top));

		for (float iz = m_bounds.bottom; iz < m_bounds.top; iz += interval)
		{
			append(Vector2f(m_bounds.left, iz));
			append(Vector2f(m_bounds.right, iz));
		}

		append(Vector2f(m_bounds.left, m_bounds.top));
		append(Vector2f(m_bounds.right, m_bounds.top));

		m_mesh->complete();

		m_mesh->bind();
		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
		// Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		m_mesh->unbind();
	}

	void append(Vector2f position)
	{
		m_mesh->addVertex(Vertex(Vector3f(position.x, 0.0f, position.y), Vector3f(0.0f, 1.0f, 0.0f)));
		m_mesh->addIndex(m_mesh->getSize()-1);
	}

	void render(Shader& shader, Camera& camera)
	{
		// shader parameters
		shader.setUniform("objectColour", 0.4f, 0.4f, 0.4f);
		shader.setUniform("lightColour", 1.0f, 1.0f, 1.0f);
		shader.setUniform("lightPos", 0.5f, 20.0f, 0.5f);
		shader.setUniform("viewPos", camera.getPosition());
		shader.setUniform("fade", 1.0f);
		shader.setUniform("wireframe", 1.0f);

		// Pass the matrices to the shader
		shader.setUniform("modelViewMatrix", getTransform());
		shader.setUniform("projectionMatrix", camera.getProjection());

		shader.bind();

		// Draw mesh
		m_mesh->draw(false);
	}

private:

	const float interval = 0.2f;
	RectF m_bounds;
	Mesh::Ptr m_mesh;
};