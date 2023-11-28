#pragma once

#include "app.hpp"
#include "constants.hpp"
#include "factories.hpp"
#include "../components/position.hpp"
#include "../components/player.hpp"
#include "../components/sprite.hpp"
#include "../components/pipe.hpp"
#include "../components/velocity.hpp"
#include "../components/gravity.hpp"
#include "../components/boundingbox.hpp"
#include "../sys/pipe_behaviour.hpp"

#include <iostream>
#include <filesystem>


Application::Application()
{
    init();
}

void Application::init()
{
    m_window.create(sf::VideoMode(width, height), "CMake SFML Project");
    m_window.setFramerateLimit(144);

    // Get the current working directory
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::cout << "Current working directory: " << currentPath << "\n";

    std::filesystem::path texturePath = std::filesystem::path("../../../../assets/sprites");

    // Check if the directory exists
    if (std::filesystem::exists(texturePath) && std::filesystem::is_directory(texturePath)) {
        // Iterate over all files in the directory
        for (auto& entry : std::filesystem::directory_iterator(texturePath)) {

            // Get the file path
            auto filePath = entry.path();

            // Access individual components of the path
            std::string directory = filePath.parent_path().string();
            std::string filename = filePath.filename().string();
            std::string stem = filePath.stem().string();
            std::string extension = filePath.extension().string();

            // print the components
            std::cout << "directory: " << directory << "\n";
            std::cout << "filename: " << filename << "\n";
            std::cout << "stem: " << stem << "\n";
            std::cout << "extension: " << extension << "\n";

            // Print the file path
            std::cout <<  "\n";

            sf::Texture texture;
            if (!texture.loadFromFile(filePath.string()))
            {
                // error..
            }
            else
            {
                m_textureMap[stem] = texture;
            }
        }
    }
    else {
        std::cerr << "Directory not found." << "\n";
    }
    sf::Sprite playerSprite;
    playerSprite.setTexture(m_textureMap["bluebird-midflap"]);
    const entt::entity player = makePlayer(m_reg, playerSprite);

    m_randomGenerator = std::mt19937(m_randomDevice());

    m_clock.restart();
}

void Application::update()
{
    m_elapsedTime = m_clock.getElapsedTime();

    sUserInput();

    // TODO: 
    // update entity manager
    sCleanUpEntities();

    // pipe spawner

    // movement
    sMovement();

    // collision
    // animation

    if (m_elapsedTime.asSeconds() >= 1)
    {
        //std::cout << "100 milliseconds have passed\n";
        //sMovement();
        sPipeSpawner();
        m_clock.restart();
    }

    // render
    sRender();
}

Application::~Application()
{

}

void Application::quit()
{
    m_running = false;
}

void Application::run()
{
    ////auto window = sf::RenderWindow{ { 288u, 512u }, "CMake SFML Project" };
    //auto window = sf::RenderWindow{ { 1920u, 1080u }, "CMake SFML Project" };
    //window.setFramerateLimit(144);

    std::cout << "Game is now running\n";

    while (isRunning())
    {
        update();
    }

    m_window.close();

    std::cout << "Game is now done running, Press Enter to continue...\n";

    //std::cin.ignore();  // Ignore any characters in the input buffer
    std::cin.get();     // Wait for the user to press Enter
}

bool Application::isRunning()
{
    return m_running && m_window.isOpen();
}

void Application::sUserInput()
{
    sf::Event evnt;
    while (m_window.pollEvent(evnt)) {

        if (evnt.type == sf::Event::Closed) {
            quit();
        }

        if (evnt.type == sf::Event::KeyPressed) {
            if (evnt.key.code == sf::Keyboard::X) {
                std::cout << "screenshot saved to " << "test.png" << "\n";
                sf::Texture texture;
                texture.create(m_window.getSize().x, m_window.getSize().y);
                texture.update(m_window);

                if (texture.copyToImage().saveToFile("test.png")) {
                    std::cout << "screenshot saved to " << "test.png" << "\n";
                }
            }
        }
    }
}

void Application::sCleanUpEntities()
{
    destroyPipe(m_reg);
}

void Application::sPipeSpawner()
{
    std::uniform_int_distribution<> distribPipeDistanceY(500, 600);
    std::uniform_int_distribution<> distribBottomPipeY(height - (height / 4.f), height);

    float pipeDistance = static_cast<float>(distribPipeDistanceY(m_randomGenerator));
    float bottomPipeY = static_cast<float>(distribBottomPipeY(m_randomGenerator));

    sf::Sprite pipeSprite;
    pipeSprite.setTexture(m_textureMap["pipe-green"]);
    auto posVec1 = sf::Vector2f(width, bottomPipeY);
    auto velVec1 = sf::Vector2f(-2.f, 0.f);
    const entt::entity pipe = makePipe(m_reg, pipeSprite, posVec1, velVec1);
    
    sf::Sprite upsideDownPipeSprite;
    upsideDownPipeSprite.setTexture(m_textureMap["pipe-green"]);
    auto posVec2 = sf::Vector2f(width, posVec1.y - pipeDistance);
    auto velVec2 = sf::Vector2f(-2.f, 0.f);

    upsideDownPipeSprite.setScale(upsideDownPipeSprite.getScale().x, -1.f * upsideDownPipeSprite.getScale().y);
    const entt::entity upsideDownPipe = makePipe(m_reg, upsideDownPipeSprite, posVec2, velVec2);
}

void Application::sRender()
{
    m_window.clear();

    backgroundRender();

    playerRender();

    pipeRender();

    collisionRender();

    m_window.display();
}

void Application::sMovement()
{
    const auto view = m_reg.view<Position, PipeSprite, Velocity>();

    for (const auto& e : view)
    {
        auto& pos = view.get<Position>(e).pos;
        auto& velocity = view.get<Velocity>(e).vel;
        pos.x += velocity.x;
    }

    const auto playerView = m_reg.view<Position, Velocity, Gravity>();

    for (const auto& e : playerView)
    {
        auto& pos = playerView.get<Position>(e).pos;
        auto& velocity = playerView.get<Velocity>(e).vel;
        auto& gravity = playerView.get<Gravity>(e).vel;
        //velocity.y += gravity.y;
        //pos.y += gravity.y;
    }
}

void Application::backgroundRender()
{
    sf::Sprite background;
    background.setTexture(m_textureMap["background-night"]);
    background.setScale(
        static_cast<float>(m_window.getSize().x) / background.getLocalBounds().width,
        static_cast<float>(m_window.getSize().y) / background.getLocalBounds().height
    );

    m_scale = background.getScale();

    m_window.draw(background);
}

void Application::playerRender()
{
    const auto view = m_reg.view<Position, BoundingBox, PlayerSprite>();

    for (const auto& e : view)
    {
        auto& pos = view.get<Position>(e).pos;
        auto& box = view.get<BoundingBox>(e);
        auto& sprite = view.get<PlayerSprite>(e).id;
        sprite.setOrigin(box.halfSize);
        sprite.setPosition(pos);
        /*sprite.setScale(
            m_scale
        );*/
        m_window.draw(sprite);
    }
}

void Application::pipeRender()
{
    const auto view = m_reg.view<Position, BoundingBox, PipeSprite>();

    for (const auto& e : view)
    {
        auto& pos = view.get<Position>(e).pos;
        auto& box = view.get<BoundingBox>(e);
        auto& sprite = view.get<PipeSprite>(e).id;
        sprite.setOrigin(box.halfSize);
        sprite.setPosition(pos);
        /*sprite.setScale(
            m_scale
        );*/
        m_window.draw(sprite);
    }
}

void Application::collisionRender() 
{
    const auto view = m_reg.view<Position, BoundingBox>();

    for (const auto& e : view)
    {
        auto& pos = view.get<Position>(e).pos;
        auto& box = view.get<BoundingBox>(e);
        sf::RectangleShape rect;
        rect.setSize(box.size);
        rect.setOrigin(box.halfSize);
        rect.setPosition(pos);
        rect.setFillColor(sf::Color(0, 0, 0, 0));
        rect.setOutlineColor(sf::Color(255, 255, 255, 255));
        rect.setOutlineThickness(1);
        m_window.draw(rect);
    }
}