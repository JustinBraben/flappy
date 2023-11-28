#pragma once

#include <entt/entity/fwd.hpp>
#include <SFML/Graphics.hpp>

entt::entity makePlayer(entt::registry& reg, sf::Sprite& playerSprite);
entt::entity makePipe(entt::registry& reg, sf::Sprite& sprite, sf::Vector2f pos, sf::Vector2f velocity);