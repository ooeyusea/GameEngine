#ifndef TEXTURE2D_H_
#define TEXTURE2D_H_
#include "../util/LLYIncludes.h"
#include <string>
#include "Resource.h"

namespace lly {

	class Texture2D : public Resource
	{
	public:
		Texture2D();
		~Texture2D();

		bool load_from_file(const std::string& filename);
		bool load_from_file(const std::string& filename, bool mipmap, ColorFormat format, TextureWrap s, TextureWrap t, TextureMinFilter min_filter, TextureMagFilter mag_filter);
		bool create(int width, int height, bool mipmap, ColorFormat format, TextureWrap s, TextureWrap t, TextureMinFilter min_filter, TextureMagFilter mag_filter);
		void attach_to_render_target(int index);

		GLuint get_tex() const { return _tex;  }

		void set_warp(TextureWrap s, TextureWrap t);
		void set_filter(TextureMinFilter min, TextureMagFilter mag);
		void gen_mipmap();

	private:
		bool load_from_data(unsigned char* data, int width, int height, bool mipmap, ColorFormat format, TextureWrap s, TextureWrap t, TextureMinFilter filter, TextureMagFilter mip_filter);

	private:
		std::string _name;
		GLuint _tex;

		ColorFormat _format;
		int _width;
		int _height;

		TextureWrap _warp_s;
		TextureWrap _warp_t;
		TextureMinFilter _min_filter;
		TextureMagFilter _mag_filter;
		bool _mipmap;
	};
}

#endif //TEXTURE2D_H_

