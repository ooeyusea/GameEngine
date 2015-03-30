#include "RenderTarget.h"
#include "../resource/Texture2D.h"
#include "../resource/RenderBuffer.h"

namespace lly {

	RenderTarget::RenderTarget()
		: _id(0)
		, _renderbuffer(nullptr)
	{
	}


	RenderTarget::~RenderTarget()
	{
		if (_id > 0)
		{
			glDeleteFramebuffers(1, &_id);
		}
	}

	bool RenderTarget::create()
	{
		glGenFramebuffers(1, &_id);
		glBindFramebuffer(GL_FRAMEBUFFER, _id);

		int index = 0;
		for (auto texture : _textures)
		{
			texture->attach_to_render_target(index++);
		}

		if (_renderbuffer)
		{
			_renderbuffer->attach_to_render_target();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return true;
	}

	void RenderTarget::load()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
	}

	void RenderTarget::unload()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
