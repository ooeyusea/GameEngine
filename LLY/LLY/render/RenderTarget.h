#ifndef RENDERTARGET_H_
#define RENDERTARGET_H_
#include "../util/LLYIncludes.h"
#include <vector>

namespace lly {
	class Texture2D;
	class RenderBuffer;

	class RenderTarget
	{
	public:
		RenderTarget();
		~RenderTarget();

		void add_render_texture(Texture2D * texture) { _textures.push_back(texture);  }
		void set_render_buffer(RenderBuffer * buffer) { _renderbuffer = buffer; }
		bool create();

		void load();
		void unload();

	private:
		GLuint _id;
		std::vector<Texture2D*> _textures;
		RenderBuffer * _renderbuffer;
	};

}

#endif //RENDERTARGET_H_



