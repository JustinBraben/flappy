#pragma once

#include "factories.hpp"
#include "../components/player.hpp"
#include "../components/position.hpp"
#include "../components/sprite.hpp"
#include "../components/pipe.hpp"

#include <entt/entity/registry.hpp>

entt::entity makePlayer(entt::registry &reg, sf::Sprite &sprite)
{
	const entt::entity player =  reg.create();
	reg.emplace<Player>(player);
	reg.emplace<Position>(player, sf::Vector2f(30.f, 30.f));
	reg.emplace<PlayerSprite>(player, sprite);
	return player;
}

entt::entity makePipe(entt::registry &reg, sf::Sprite& sprite, sf::Vector2f& pos)
{
	const entt::entity pipe = reg.create();
	reg.emplace<Player>(pipe);
	reg.emplace<Position>(pipe, pos);
	reg.emplace<PipeSprite>(pipe, sprite);
	return pipe;
}