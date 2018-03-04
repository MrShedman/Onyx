#pragma once

#include "GL\glew.h"

#include "rendering\Transform.hpp"
#include "rendering\Shader.hpp"
#include "rendering\Camera.hpp"
#include "rendering\Model.hpp"
#include "rendering\Ground.hpp"
#include "rendering\Sphere.hpp"

#include "SFML\Window\Event.hpp"

namespace sf
{
	class Window;
}

class GraphicSystem
{
public:

	void init(sf::Window* window);

	void render();

	void handleEvent(const sf::Event& event, const sf::Time& dt)
	{
		camera.handleEvent(event, dt);

		// Adjust the viewport when the window is resized
		if (event.type == sf::Event::Resized)
		{
			glViewport(0, 0, event.size.width, event.size.height);
		}
	}

	void update(const sf::Time& dt)
	{
		camera.Update(dt);
	}

private:

	sf::Window* window;

	Shader modelShader;

	Camera camera;

	Ground ground;
};