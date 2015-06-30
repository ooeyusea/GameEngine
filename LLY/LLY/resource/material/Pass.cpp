#include "Pass.h"
#include "../../util/Macros.h"
#include "../Program.h"
#include "../MeshPart.h"
#include "../../render/RenderCommand.h"
#include "../../System.h"

namespace lly {

	Pass::Pass()
		: _id(0)
		, _program(0)
	{
	}


	Pass::~Pass()
	{
		if (_program > 0)
		{
			_program = 0;
		}
	}

	void Pass::draw(MeshPart& obj)
	{
		if (obj.get_index_buffer() != nullptr)
		{
			RenderCommand* command = new RenderCommand;
			if (command)
			{
				command->set_program(System::instance().get_resource_manager().get_program(_program));
				command->set_index_buffer(obj.get_index_buffer());
				command->set_render_option(_option.to_render());
				System::instance().add_render_command(command);
			}
		}
	}

	void Pass::draw(IndexBuffer* index)
	{
		RenderCommand* command = new RenderCommand;
		if (command)
		{
			command->set_program(System::instance().get_resource_manager().get_program(_program));
			command->set_index_buffer(index);
			command->set_render_option(_option.to_render());
			System::instance().add_render_command(command);
		}
	}

	void Pass::merge_render_option(const RenderOption& option)
	{
		_option.merge(option);
	}

	Pass * Pass::clone()
	{
		Pass * ret = new Pass;
		ret->_id = _id;
		ret->_program = _program;
		ret->_option = _option;

		return ret;
	}
}
