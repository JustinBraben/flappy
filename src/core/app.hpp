#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entity/registry.hpp>
#include <random>

class Application {
protected:
	sf::RenderWindow m_window;
	bool m_running = true;
	bool m_paused = false;
	bool m_collided = false;
	bool m_drawCollision = false;
	int m_score;
	std::map<std::string, sf::Texture> m_textureMap;
	std::map<std::string, sf::Font> m_fontMap;
	sf::Vector2f m_scale;
	entt::registry m_reg;
	sf::Clock m_clock;
	sf::Time m_elapsedTime;
	std::random_device m_randomDevice;
	std::mt19937 m_randomGenerator;

	
	void init();
	void loadFonts();
	void loadTextures();
	void restart();
	void update();

	void sUserInput();
	void sCleanUpEntities();
	void sPipeSpawner();
	void sRender();
	void sMovement();
	void sScore();
	void sCollision();

	sf::Vector2f checkOverlap(const entt::entity& a, const entt::entity& b);
	void backgroundRender();
	void playerRender();
	void pipeRender();
	void collisionRender();
public:
	Application();
	~Application();

	void quit();
	void run();

	bool isRunning();
};
