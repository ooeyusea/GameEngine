#ifndef TEXTURE3D_H_
#define TEXTURE3D_H_
#include "../util/LLYIncludes.h"
#include <string>
#include "Resource.h"

namespace lly {

	class Texture3D : public Resource
	{
	public:
		Texture3D();
		~Texture3D();

		bool load_from_file(const std::string& filename, int width, int height, int depth);
		bool load_from_file(const std::string& filename, int width, int height, int depth, bool mipmap, ColorFormat format, TextureMinFilter min_filter, TextureMagFilter mag_filter);

		GLuint get_tex() const { return _tex; }

	private:
		bool load_from_data(unsigned char* data, int width, int height, int depth, bool mipmap, ColorFormat format, TextureMinFilter min_filter, TextureMagFilter mag_filter);

	private:
		GLuint _tex;
	};
}

#endif //TEXTURE3D_H_

