#pragma once

#include "..\math\Matrix.hpp"
#include "..\math\MathHelper.hpp"
#include "..\math\Angle.hpp"

#include "Transform.hpp"

namespace sf
{
	class Window;
	class Event;
	class Time;
}

class Camera : public Transform
{
public:

	void init(sf::Window* w);

	void handleEvent(const sf::Event& event, const sf::Time& dt);

	void Update(const sf::Time& dt);

	bool isEngaged() const
	{
		return mouseClipped;
	}

	Vector3f getLookAt() const
	{
		Matrix4f mat = getTransform();
		return Vector3f(mat[2][0], mat[2][1], mat[2][2]);
		//return m_transform.getRotation().GetForward();
	}

	float getNearClippingDistance() const
	{
		return m_nearPlaneDistance;
	}

	float getFarClippingDistance() const
	{
		return m_farPlaneDistance;
	}

	Angle getFOV() const
	{
		return m_fov;
	}

	float getAspectRatio() const
	{
		return m_aspect;
	}
	
	Matrix4f getProjection() const
	{
		Matrix4f cameraRotation = getRotation().Conjugate().ToRotationMatrix();

		// The translation is inverted because the world appears to move opposite
		// to the camera's movement.
		Matrix4f cameraTranslation = Matrix4f().InitTranslation(getPosition().inverted());

		return m_projection * cameraRotation * cameraTranslation;
	}

private:

	bool mouseClipped;

	Angle m_fov;
	float m_aspect;
	float m_nearPlaneDistance;
	float m_farPlaneDistance;

	sf::Window*	m_window;

	mutable Matrix4f m_projection;
};