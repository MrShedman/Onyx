
#include "Application.hpp"

#include "SFML\Window\Event.hpp"

#include <string>
#include <iostream>
#include <random>

Application::Application()
{
	sf::ContextSettings settings;
	settings.majorVersion = 4;
	settings.minorVersion = 5;
	settings.depthBits = 24;
	settings.antialiasingLevel = 4;
	settings.attributeFlags = settings.Core;

	std::cout << "\nCreating Window..." << std::endl;

	// Create window first so when we init opencl we can use the opengl context
	window.create(sf::VideoMode(1280, 720), "Onyx", sf::Style::Default, settings);

	std::cout << "\nInitialising GLEW..." << std::endl;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
	}

	m_isOpen = (GLEW_OK == err);

	graphics.init(&window);
}

void Application::getInput()
{
	sf::Event event;

	while (window.pollEvent(event))
	{
		graphics.handleEvent(event, timePerFrame);

		// Close window: exit
		if (event.type == sf::Event::Closed)
		{
			close();
		}
		// Escape key: exit
		if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
		{
			close();
		}
		if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::BackSpace))
		{
			//capture.create(window.getSize().x, window.getSize().y);
		}
	}
}

void Application::render()
{	
	// Clear the depth buffer
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	graphics.render();

	window.display();
}

void Application::run()
{
	// Create a clock for measuring the time elapsed
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	// Start loop
	while (m_isOpen)
	{
		sf::Time dt;
		dt = clock.restart();
		timeSinceLastUpdate += dt;

		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;

			getInput();

			graphics.update(timePerFrame);
		}

		render();

		capture.writeFrame();
	}

	capture.close();

	window.close();
}