#pragma once

#include "factories.hpp"

#include "../core/constants.hpp"
#include "../components/player.hpp"
#include "../components/position.hpp"
#include "../components/sprite.hpp"
#include "../components/pipe.hpp"
#include "../components/velocity.hpp"
#include "../components/gravity.hpp"
#include "../components/boundingbox.hpp"

#include <entt/entity/registry.hpp>

entt::entity makePlayer(entt::registry &reg, sf::Sprite &sprite)
{
	sf::Vector2f size = sf::Vector2f(
		sprite.getTexture()->getSize().x,
		sprite.getTexture()->getSize().y
	);
	sf::Vector2f halfSize = sf::Vector2f(
		sprite.getTexture()->getSize().x / 2,
		sprite.getTexture()->getSize().y / 2
	);
	sf::Vector2f velocity = { 0.f, 0.f };
	sf::Vector2f gravity = { 0.f, 2.f };
	const entt::entity player =  reg.create();
	reg.emplace<Player>(player);
	reg.emplace<Position>(player, sf::Vector2f(playerSpawnX, playerSpawnY));
	reg.emplace<PlayerSprite>(player, sprite);
	reg.emplace<Velocity>(player, velocity);
	reg.emplace<BoundingBox>(player, size, halfSize);
	reg.emplace<Gravity>(player, gravity);
	return player;
}

entt::entity makePipe(entt::registry &reg, sf::Sprite& sprite, sf::Vector2f pos, sf::Vector2f velocity)
{
	sf::Vector2f size = sf::Vector2f(
		sprite.getTexture()->getSize().x, 
		sprite.getTexture()->getSize().y
	);
	sf::Vector2f halfSize = sf::Vector2f(
		sprite.getTexture()->getSize().x / 2,
		sprite.getTexture()->getSize().y / 2
	);
	const entt::entity pipe = reg.create();
	reg.emplace<Pipe>(pipe);
	reg.emplace<Position>(pipe, pos);
	reg.emplace<PipeSprite>(pipe, sprite);
	reg.emplace<Velocity>(pipe, velocity);
	reg.emplace<BoundingBox>(pipe, size, halfSize);
	return pipe;
}