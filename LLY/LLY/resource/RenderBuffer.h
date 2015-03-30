#ifndef RENDERBUFFER_H_
#define RENDERBUFFER_H_
#include "../util/LLYIncludes.h"

namespace lly {
	class RenderBuffer
	{
	public:
		enum class Usage
		{
			DEPTH,
			DEPTH_STENCIL,
		};

		RenderBuffer();
		~RenderBuffer();

		bool create(int width, int height, Usage usage);
		void attach_to_render_target();
		
	private:
		GLuint _id;
		int _width;
		int _height;
		Usage _usage;
	};
}

#endif //RENDERBUFFER_H_
