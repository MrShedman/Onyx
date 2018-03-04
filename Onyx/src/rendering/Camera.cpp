#include "Camera.hpp"

#include "SFML\Window\Window.hpp"
#include "SFML\Window\Event.hpp"

void Camera::init(sf::Window* w)
{
	mouseClipped = false;

	m_fov = degrees(70.0f);
	m_aspect = static_cast<float>(w->getSize().x) / static_cast<float>(w->getSize().y);
	m_nearPlaneDistance = 0.1f;
	m_farPlaneDistance = 1000.0f;

	m_window = w;

	m_projection.InitPerspective(m_fov.asRadians(), m_aspect, m_nearPlaneDistance, m_farPlaneDistance);
}

void Camera::handleEvent(const sf::Event& event, const sf::Time& dt)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			mouseClipped = true;

			m_window->setRawMouseInput(true);
			m_window->setMouseCursorVisible(false);
			m_window->setClippedCursor(mouseClipped);
		}
	}
	else if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::E)
		{
			mouseClipped = false;

			m_window->setRawMouseInput(false);
			m_window->setMouseCursorVisible(true);
			m_window->setClippedCursor(mouseClipped);

			auto size = m_window->getSize();
			size.x /= 2;
			size.y /= 2;

			sf::Mouse::setPosition(sf::Vector2i(size.x, size.y), *m_window);
		}
	}
	else if (event.type == sf::Event::MouseMotion)
	{
		float scale = static_cast<float>(dt.asSeconds()) / 3.0f;

		rotate(Vector3f::yAxis(), radians(event.mouseMotion.dx * scale));

		rotate(getRotation().GetRight(), radians(event.mouseMotion.dy * scale));
	}
}

void Camera::Update(const sf::Time& dt)
{
	float mult = 1.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		mult = 3.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		mult = 0.33f;
	}

	float movAmt = static_cast<float>(dt.asSeconds()) * mult;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		move(Vector3f(0, 1, 0) * movAmt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		move(getRotation().GetForward() * movAmt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		move(getRotation().GetBack() * movAmt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		move(getRotation().GetLeft() * movAmt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		move(getRotation().GetRight() * movAmt);
	}
}