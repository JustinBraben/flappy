#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entity/registry.hpp>

class Application {
protected:
	sf::RenderWindow m_window;
	bool m_running = true;
	std::map<std::string, sf::Texture> m_textureMap;
	sf::Vector2f m_scale;
	entt::registry m_reg;
	
	void init();
	void update();

	void sUserInput();
	void sRender();

	void backgroundRender();
	void playerRender();
	void pipeRender();
public:
	Application();
	~Application();

	void quit();
	void run();

	bool isRunning();
};
