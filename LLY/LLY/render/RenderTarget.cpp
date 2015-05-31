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
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _id);

		int index = 0;
		for (auto texture : _textures)
		{
			texture->attach_to_render_target(index++);
		}

		if (_renderbuffer)
		{
			_renderbuffer->attach_to_render_target();
		}

		auto status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		return status == GL_FRAMEBUFFER_COMPLETE;
	}

	void RenderTarget::load()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
		GLenum drawBuffers[16] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, 
			GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7 };
		glDrawBuffers(_textures.size(), drawBuffers);
	}

	void RenderTarget::unload()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
