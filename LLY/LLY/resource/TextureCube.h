#ifndef TEXTURECUBE_H_
#define TEXTURECUBE_H_
#include "../util/LLYIncludes.h"
#include <string>
#include "Resource.h"

namespace lly {

	class TextureCube : public Resource
	{
	public:
		TextureCube();
		~TextureCube();

		bool load_from_file(const std::string& filename);
		bool load_from_file(const std::string& filename, bool mipmap, ColorFormat format, TextureMinFilter filter, TextureMagFilter mip_filter);

		GLuint get_tex() const { return _tex; }

	private:
		bool load_from_data(unsigned char* data, int width, int height, bool mipmap, ColorFormat format, TextureMinFilter filter, TextureMagFilter mip_filter);

	private:
		GLuint _tex;
	};
}

#endif //TEXTURECUBE_H_

