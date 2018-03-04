#include "GraphicSystem.hpp"

#include "SFML\Window\Window.hpp"

void GraphicSystem::init(sf::Window* window)
{
	this->window = window;

	// Enable Z-buffer read and write
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); // for wireframe rendering
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);

	std::cout << "\nLoading Shaders..." << std::endl;

	if (!modelShader.loadFromFile("./res/shaders/model.vert", "./res/shaders/model.frag"))
	{

	}

	camera.init(window);
	camera.setPosition(0.5f, 0.75f, 1.6f);
	camera.rotate(Vector3f::yAxis(), degrees(180));
	camera.rotate(Vector3f::xAxis(), degrees(-20));

	ground.create();
}

void GraphicSystem::render()
{
	ground.render(modelShader, camera);
}