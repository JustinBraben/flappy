#pragma once

#include <entt/entity/fwd.hpp>
#include <SFML/Graphics.hpp>

entt::entity makePlayer(entt::registry& reg, sf::Sprite& playerSprite);
entt::entity makePipe(entt::registry& reg, entt::entity);