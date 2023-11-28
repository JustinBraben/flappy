#include <iostream>
#include <filesystem>
#include <map>
#include <string>

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

struct position {
    float x;
    float y;
};

struct velocity {
    float dx;
    float dy;
};

void update(entt::registry& registry) {
    auto view = registry.view<const position, velocity>();

    // use a callback
    view.each([](const auto& pos, auto& vel) { /* ... */ });

    // use an extended callback
    view.each([](const auto entity, const auto& pos, auto& vel) { /* ... */ });

    // use a range-for
    for (auto [entity, pos, vel] : view.each()) {
        // ...
    }

    // use forward iterators and get only the components of interest
    for (auto entity : view) {
        auto& vel = view.get<velocity>(entity);
        // ...
    }
}

int main()
{
    entt::registry registry;

    for (auto i = 0u; i < 10u; ++i) {
        const auto entity = registry.create();
        registry.emplace<position>(entity, i * 1.f, i * 1.f);
        if (i % 2 == 0) { registry.emplace<velocity>(entity, i * .1f, i * .1f); }
    }

    update(registry);

    //auto window = sf::RenderWindow{ { 1920u, 1080u }, "CMake SFML Project" };
    auto window = sf::RenderWindow{ { 288u, 512u }, "CMake SFML Project" };
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

    sf::Sprite back; 
    back.setTexture(textureMap["background-night"]);
    //back.setPosition(sf::Vector2f(0.f, 0.f));

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

        window.draw(back);

        window.display();
    }
}