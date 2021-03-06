#pragma once

#include "ecs/system.h"


namespace pulsar
{
	class script_system : public pulsar::ecs::system
	{
	public:
		script_system() = default;
		~script_system() = default;

		void	execute(pulsar::ecs::registry &registry, float delta_time) override;
	};
}
