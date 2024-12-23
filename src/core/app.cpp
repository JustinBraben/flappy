#pragma once

#include "app.hpp"
#include "constants.hpp"
#include "factories.hpp"
#include "../components/boundingbox.hpp"
#include "../components/gravity.hpp"
#include "../components/jump.hpp"
#include "../components/passedbird.hpp"
#include "../components/pipe.hpp"
#include "../components/player.hpp"
#include "../components/position.hpp"
#include "../components/sprite.hpp"
#include "../components/velocity.hpp"
#include "../sys/pipe_behaviour.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>


Application::Application()
{
    init();
}

void Application::init()
{
    m_window.create(sf::VideoMode(sf::Vector2u(width, height)), sf::String("Flappy window"));
    m_window.setFramerateLimit(144);

    loadTextures();

    loadFonts();

    restart();

    m_randomGenerator = std::mt19937(m_randomDevice());

    m_clock.restart();
}

void Application::loadFonts()
{
    // Get the current working directory
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::cout << "Current working directory: " << currentPath << "\n";

    std::filesystem::path fontPath = std::filesystem::path(fontLoadPath);

    // Check if the directory exists
    if (std::filesystem::exists(fontPath) && std::filesystem::is_directory(fontPath)) {
        // Iterate over all files in the directory
        for (auto& entry : std::filesystem::directory_iterator(fontPath)) {

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
            std::cout << "\n";

            sf::Font font;
            if (!font.openFromFile(filePath.string()))
            {
                // error..
            }
            else
            {
                m_fontMap[stem] = font;
            }
        }
    }
    else {
        std::cerr << "Directory not found." << "\n";
    }
}

void Application::loadTextures()
{
    // Get the current working directory
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::cout << "Current working directory: " << currentPath << "\n";

    std::filesystem::path texturePath = std::filesystem::path(textureLoadPath);

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
}

void Application::restart()
{
    auto playerView = m_reg.view<Player, Position, PlayerSprite, Velocity, BoundingBox, Gravity, Jump>();
    
    for (auto& player : playerView)
    {
        m_reg.remove<Player, Position, PlayerSprite, Velocity, BoundingBox, Gravity, Jump>(player);
    }

    auto pipeView = m_reg.view<Pipe, Position, PipeSprite, Velocity, BoundingBox, PassedBird>();
    for (auto& pipe : pipeView)
    {
        m_reg.remove<Pipe, Position, PipeSprite, Velocity, BoundingBox, PassedBird>(pipe);
    }

    auto playerSprite = sf::Sprite(m_textureMap["bluebird-midflap"]);
    const entt::entity player = makePlayer(m_reg, playerSprite);

    m_score = 0;
    m_collided = false;
}

void Application::update()
{
    sUserInput();

    if (m_running)
    {
        m_elapsedTime = m_clock.getElapsedTime();

        sCleanUpEntities();

        if (m_elapsedTime.asSeconds() >= 1)
        {
            //std::cout << "100 milliseconds have passed\n";
            sPipeSpawner();
            m_clock.restart();
        }
    }

    if (m_collided)
    {
        // TODO:
        // draw Game Over + score

        // Press R to play again
        // Esc to quit
    }
    else
    {
        sMovement();
    }

    sCollision();

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
    while (const std::optional evnt = m_window.pollEvent()) {

        if (evnt->is<sf::Event::Closed>()) {
            quit();
        }

        if (const auto* keyPressed = evnt->getIf<sf::Event::KeyPressed>()) {

            if (keyPressed->scancode == sf::Keyboard::Scancode::X) {
                std::cout << "screenshot saved to " << "test.png" << "\n";
                auto texture = sf::Texture(m_window.getSize());
                texture.update(m_window);

                if (texture.copyToImage().saveToFile("test.png")) {
                    std::cout << "screenshot saved to " << "test.png" << "\n";
                }
            }

            if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
            {
                const auto view = m_reg.view<PlayerSprite, Velocity, Jump>();
                for (const auto& e : view)
                {
                    if (view.get<Jump>(e).has)
                    {
                        auto& velocity = view.get<Velocity>(e).vel;
                        auto& canJump = view.get<Jump>(e).has;
                        velocity.y = 0.f;
                        velocity.y += playerJumpVelocityY;
                        canJump = false;
                    }
                }
            }
        }

        if (const auto* keyReleased = evnt->getIf<sf::Event::KeyReleased>())
        {
            if (keyReleased->scancode == sf::Keyboard::Scancode::Space)
            {
                const auto view = m_reg.view<PlayerSprite, Jump>();
                for (const auto& e : view)
                {
                    auto& canJump = view.get<Jump>(e).has;
                    canJump = true;
                }
            }

            if (keyReleased->scancode == sf::Keyboard::Scancode::C)
            {
                if (m_drawCollision)
                {
                    m_drawCollision = false;
                }
                else
                {
                    m_drawCollision = true;
                }
            }

            if (keyReleased->scancode == sf::Keyboard::Scancode::R)
            {
                restart();
            }

            if (keyReleased->scancode == sf::Keyboard::Scancode::Escape)
            {
                m_running = false;
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

    auto pipeSprite = sf::Sprite(m_textureMap["pipe-green"]);
    auto posVec1 = sf::Vector2f(width, bottomPipeY);
    auto velVec1 = sf::Vector2f(-2.f, 0.f);
    const entt::entity pipe = makePipe(m_reg, pipeSprite, posVec1, velVec1);
    
    auto upsideDownPipeSprite = sf::Sprite(m_textureMap["pipe-green"]);
    auto posVec2 = sf::Vector2f(width, posVec1.y - pipeDistance);
    auto velVec2 = sf::Vector2f(-2.f, 0.f);

    upsideDownPipeSprite.setScale(
        sf::Vector2f(
            upsideDownPipeSprite.getScale().x, 
            -1.f * upsideDownPipeSprite.getScale().y
        )
    );
    const entt::entity upsideDownPipe = makePipe(m_reg, upsideDownPipeSprite, posVec2, velVec2);
}

void Application::sRender()
{
    m_window.clear();

    backgroundRender();

    playerRender();

    pipeRender();

    if (m_drawCollision)
    {
        collisionRender();
    }

    if (m_collided)
    {
        gameOverRender();
    }

    sScore();

    m_window.display();
}

void Application::sMovement()
{
    const auto view = m_reg.view<Position, PipeSprite, Velocity, PassedBird>();

    for (const auto& e : view)
    {
        auto& pos = view.get<Position>(e).pos;
        auto& velocity = view.get<Velocity>(e).vel;
        auto& hasPassedBird = view.get<PassedBird>(e).has;
        auto& sprite = view.get<PipeSprite>(e).id;
        pos.x += velocity.x;

        if (!hasPassedBird && 
           (pos.x < ((width / 2.f) - (sprite.getTexture().getSize().x / 2.f))) &&
            pos.y <= height - (height / 4.f))
        {
            m_score += 1;
            hasPassedBird = true;
        }
    }

    const auto playerView = m_reg.view<Position, Velocity, Gravity>();

    for (const auto& e : playerView)
    {
        auto& pos = playerView.get<Position>(e).pos;
        auto& velocity = playerView.get<Velocity>(e).vel;
        auto& gravity = playerView.get<Gravity>(e).vel;
        velocity.y += gravity.y;
        velocity.y = std::clamp(velocity.y, playerMinVelocityY, playerMaxVelocityY);
        pos.y += velocity.y;

        if (pos.y < 0.f || pos.y > height)
        {
            m_collided = true;
            //restart();
        }
    }
}

void Application::sScore()
{
    // TODO:
    // get the score
    // turn it into a string
    // split them out individually into a vector of strings
    std::string scoreString = std::to_string(m_score);
    std::vector<std::string> letters;

    for (auto letter : scoreString)
    {
        letters.push_back(std::string(1, letter));
    }

    sf::Vector2f scoreMiddleLocation = { width / 2.f, 100.f };

    std::vector<sf::Sprite> scoreSprites;

    for (const auto& letter : letters)
    {
        auto& texture = m_textureMap[letter];
        auto scoreSprite = sf::Sprite(texture);
        scoreSprite.setTexture(texture);
        scoreSprite.setOrigin(
            sf::Vector2f(
                static_cast<float>(texture.getSize().x) / 2.f,
                static_cast<float>(texture.getSize().y) / 2.f
            )
        );
        scoreSprites.push_back(scoreSprite);
    }

    for (auto& sprite : scoreSprites)
    {
        sprite.setPosition(
            sf::Vector2f(
                scoreMiddleLocation.x,
                scoreMiddleLocation.y
            )
        );
        scoreMiddleLocation.x += sprite.getTexture().getSize().x;
        m_window.draw(sprite);
    }
}

void Application::sCollision()
{
    const auto pipeView = m_reg.view<Position, BoundingBox, PipeSprite>();

    for (const auto& e : pipeView)
    {
        auto playerView = m_reg.view<Position, Velocity, PlayerSprite, BoundingBox>();

        for (const auto& player : playerView)
        {
            auto overlap = checkOverlap(player, e);

            if (overlap.x > 0 && overlap.y > 0)
            {
                auto& velocity = playerView.get<Velocity>(player);
                velocity.vel.y = 0;
                m_collided = true;
                //restart();
                //m_running = false;
            }
        }
    }
}

sf::Vector2f Application::checkOverlap(const entt::entity& a, const entt::entity& b)
{
    auto& aBox = m_reg.get<BoundingBox>(a);
    auto& aPos = m_reg.get<Position>(a);
    auto& bBox = m_reg.get<BoundingBox>(b);
    auto& bPos = m_reg.get<Position>(b);

    sf::Vector2f delta = { std::abs(aPos.pos.x - bPos.pos.x) ,
                        std::abs(aPos.pos.y - bPos.pos.y) };

    auto overlapX = aBox.halfSize.x + bBox.halfSize.x - delta.x;
    auto overlapY = aBox.halfSize.y + bBox.halfSize.y - delta.y;

    auto overlap = sf::Vector2f(overlapX, overlapY);

    return overlap;
}

void Application::backgroundRender()
{
    sf::Sprite background = sf::Sprite(m_textureMap["background-night"]);
    background.setScale( sf::Vector2f(
        static_cast<float>(m_window.getSize().x) / background.getLocalBounds().size.x,
        static_cast<float>(m_window.getSize().y) / background.getLocalBounds().size.y
    ));

    m_scale = background.getScale();

    m_window.draw(background);
}

void Application::playerRender()
{
    const auto view = m_reg.view<Position, BoundingBox, Velocity, PlayerSprite>();

    for (const auto& e : view)
    {
        auto& pos = view.get<Position>(e).pos;
        auto& box = view.get<BoundingBox>(e);
        auto& velocity = view.get<Velocity>(e).vel;
        auto& sprite = view.get<PlayerSprite>(e).id;
        sprite.setOrigin(box.halfSize);
        sprite.setRotation(sf::degrees(velocity.y * 40.f));
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

void Application::gameOverRender()
{
    auto gameOverText = sf::Text(m_fontMap["AboveDemoRegular-lJMd"], "GAME OVER\nPress R to restart\nPress Esc to quit");
    gameOverText.setString("GAME OVER\nPress R to restart\nPress Esc to quit");
    gameOverText.setFont(m_fontMap["AboveDemoRegular-lJMd"]);
    gameOverText.setCharacterSize(32);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setOrigin(
        sf::Vector2f(
            gameOverText.getLocalBounds().size.x / 2.0f,
            gameOverText.getLocalBounds().size.y / 2.0f
        )
    );
    gameOverText.setPosition(
        sf::Vector2f(
            width / 2.f, 
            height / 2.f
        )
    );
    m_window.draw(gameOverText);
}