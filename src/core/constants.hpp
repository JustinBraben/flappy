#include <SFML/Graphics.hpp>
#include <filesystem>
#include <string>

constexpr float pipeDistanceMinY = 450.f;

constexpr float pipeDistanceMaxY = 500.f;

constexpr float pipeDistanceMinX = 450.f;

constexpr float pipeDistanceMaxX = 500.f;

constexpr float width = 1280.f;

constexpr float height = 600.f;

constexpr auto textureLoadPath = "../assets/sprites";

constexpr auto fontLoadPath = "../assets/fonts";

constexpr float playerSpawnX = width / 2.0f;

constexpr float playerSpawnY = height / 2.0f;

constexpr float playerMaxVelocityY = 2.f;

constexpr float playerMinVelocityY = -2.f;

constexpr float playerJumpVelocityY = -1.f;

constexpr float gravityVelocity = 0.03f;