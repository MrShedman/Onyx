#pragma once

#include "GL\glew.h"

#include "GraphicSystem.hpp"
#include "rendering\Capture.hpp"

#include "SFML\Window\Window.hpp"

class Application
{
public:

	Application();

	void run();

	void close()
	{
		m_isOpen = false;
	}

private:

	bool m_isOpen;

	const sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);

	void getInput();
	void render();

	Capture capture;

	GraphicSystem graphics;

	sf::Window window;
};