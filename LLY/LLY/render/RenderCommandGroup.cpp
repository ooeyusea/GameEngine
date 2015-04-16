#include "RenderCommandGroup.h"
#include "Device.h"
#include "RenderCommand.h"

namespace lly {

	RenderCommandGroup::RenderCommandGroup()
		: _target(nullptr)
	{
	}


	RenderCommandGroup::~RenderCommandGroup()
	{
		for (auto& command : _commands)
		{
			delete command;
		}
		_commands.clear();
	}

	bool RenderCommandGroup::operator<(const RenderCommandGroup& rhs) const
	{
		return true;
	}

	void RenderCommandGroup::draw(Device& device)
	{
		device.begin(_target, ClearTarget::COLOR | ClearTarget::DEPTH);
		for (auto& command : _commands)
		{
			command->draw(device);
		}
		device.end(_target);
	}
}
