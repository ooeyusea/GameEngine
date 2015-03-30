#include "RenderCommand.h"
#include "Device.h"
#include "../resource/material/RenderOption.h"
#include "../resource/material/Uniform.h"
#include "../resource/IndexBuffer.h"

namespace lly {

	RenderCommand::RenderCommand()
		: _batch(0)
	{
	}


	RenderCommand::~RenderCommand()
	{
	}

	bool RenderCommand::operator<(const RenderCommand& rhs) const
	{
		return _batch < rhs._batch;
	}

	void RenderCommand::draw(Device& device)
	{
		if (_program == nullptr)
			return;

		device.use_program(_program);
		_option.use(device);

		_index->draw(device);
	}
}

