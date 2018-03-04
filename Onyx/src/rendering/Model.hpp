#pragma once

#include "Transform.hpp"
#include "Mesh.hpp"

#include <memory>
#include <string>

class Shader;
class Camera;

class Model : public Transform
{
public:

	Model(){}

	Model(const std::string& filename)
	{
		loadFromFile(filename);
	}

	Model(Mesh::Ptr mesh)
	{
		m_mesh = mesh;
	}

	Model(const Model& other) = delete;

	Model& operator=(const Model& other) = delete;

	Model(Model&& other) :
		Transform(std::move(other)),
		m_mesh(std::move(other.m_mesh))
	{}

	Model& operator=(Model&& other)
	{
		if (this != &other)
		{
			Transform::operator=(std::move(other));
			m_mesh = std::move(other.m_mesh);
		}

		return *this;
	}

	AABBf getLocalBounds() const
	{
		return m_mesh->getLocalBounds();
	}

	AABBf getGlobalBounds() const
	{
		return m_mesh->getGlobalBounds(getTransform());
	}

	float getVolume() const
	{
		return m_mesh->getVolume(getTransform());
	}

	void loadFromFile(const std::string& filename);

	void saveToFile(const std::string& filename);

	void setColour(const Vector3f& colour)
	{
		m_colour = colour;
	}

	Vector3f getColour() const
	{
		return m_colour;
	}

	void generateNormals()
	{
		m_mesh->updateNormals();
	}

	void render(Shader& shader, Camera& camera, bool wireframe = false);

private:

	Vector3f m_colour;

	Mesh::Ptr m_mesh;
};