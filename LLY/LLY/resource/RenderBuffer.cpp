#include "RenderBuffer.h"

namespace lly {

	GLenum to_usage(RenderBuffer::Usage usage)
	{
		if (usage == RenderBuffer::Usage::DEPTH)
			return GL_DEPTH_COMPONENT;
		return GL_DEPTH24_STENCIL8;
	}

	RenderBuffer::RenderBuffer()
	{

	}

	RenderBuffer::~RenderBuffer()
	{
		if (_id > 0)
		{
			glDeleteRenderbuffers(1, &_id);
			_id = 0;
		}
	}

	bool RenderBuffer::create(int width, int height, Usage usage)
	{
		glGenRenderbuffers(1, &_id);
		glBindRenderbuffer(GL_RENDERBUFFER, _id);
		glRenderbufferStorage(GL_RENDERBUFFER, to_usage(usage), (GLsizei)width, (GLsizei)height);

		_width = width;
		_height = height;
		_usage = usage;

		return true;
	}

	void RenderBuffer::attach_to_render_target()
	{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _id);
		if (_usage == Usage::DEPTH_STENCIL)
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _id);
	}
}