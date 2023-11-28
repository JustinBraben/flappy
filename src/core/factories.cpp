#pragma once

#include "factories.hpp"
#include "../components/player.hpp"
#include "../components/position.hpp"
#include "../components/sprite.hpp"

#include <entt/entity/registry.hpp>

entt::entity makePlayer(entt::registry &reg, sf::Sprite &playerSprite)
{
	const entt::entity player =  reg.create();
	reg.emplace<Player>(player);
	reg.emplace<Position>(player, sf::Vector2f(30.f, 30.f));
	reg.emplace<PlayerSprite>(player, playerSprite);
	return player;
}

entt::entity makePipe(entt::registry &reg, entt::entity)
{
	return entt::entity();
}