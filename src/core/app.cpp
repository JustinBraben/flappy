#include "app.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>

Application::Application()
{
}

Application::~Application()
{
}

void Application::run()
{
    //auto window = sf::RenderWindow{ { 288u, 512u }, "CMake SFML Project" };
    auto window = sf::RenderWindow{ { 1920u, 1080u }, "CMake SFML Project" };
    window.setFramerateLimit(144);

    // Get the current working directory
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::cout << "Current working directory: " << currentPath << "\n";

    std::map<std::string, sf::Texture> textureMap;
    std::filesystem::path texturePath = "../../../../assets/sprites";

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
                textureMap[stem] = texture;
            }
        }
    }
    else {
        std::cerr << "Directory not found." << "\n";
    }
    
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("../../../../assets/sprites/background-day.png"))
    {
        // error..
    }

    sf::Sprite background; 
    background.setTexture(textureMap["background-night"]);
    background.setScale(
        static_cast<float>(window.getSize().x) / background.getLocalBounds().width, 
        static_cast<float>(window.getSize().y) / background.getLocalBounds().height
    );

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();

        window.draw(background);

        window.display();
    }

    std::cout << "hello\n";

    // Declare a string to store user input
    std::string userInput;

    // Use std::getline to read a line of input (including spaces)
    std::getline(std::cin, userInput);
}
