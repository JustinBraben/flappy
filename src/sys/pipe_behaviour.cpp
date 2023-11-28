#include "pipe_behaviour.hpp"

#include "../components/position.hpp"
#include "../components/sprite.hpp"
#include "../components/pipe.hpp"
#include "../components/velocity.hpp"
#include "../components/gravity.hpp"
#include <entt/entity/registry.hpp>

void destroyPipe(entt::registry &reg)
{
	const auto view = reg.view<Pipe, Position>();
	for (const auto e : view)
	{
		auto& pos = view.get<Position>(e);
		if (pos.pos.x < 0.f)
		{
			reg.remove<Position>(e);
			reg.remove<PipeSprite>(e);
			reg.remove<Pipe>(e);
			reg.remove<Velocity>(e);
		}
	}
}
