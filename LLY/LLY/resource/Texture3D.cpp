#include "Texture3D.h"
#include "../util/FileUtil.h"
#include <png.h>
#include "Image.h"
#include "../util/FuncUnitl.h"

namespace lly {

	Texture3D::Texture3D()
		: _tex(0)
	{
	}


	Texture3D::~Texture3D()
	{
		if (_tex > 0)
		{
			glDeleteTextures(1, &_tex);
			_tex = 0;
		}
	}

	bool Texture3D::load_from_file(const std::string& filename, int width, int height, int depth)
	{
		return load_from_file(filename, width, height, depth, true, ColorFormat::DEFAULT, TextureMinFilter::LINEAR, TextureMagFilter::LINEAR);
	}

	bool Texture3D::load_from_file(const std::string& filename, int width, int height, int depth, bool mipmap, ColorFormat format, TextureMinFilter min_filter, TextureMagFilter mag_filter)
	{
		Image image;
		if (!image.load_from_file(filename))
			return false;

		if (!image.change_color_format(format))
			return false;

		return load_from_data(image.data(), width, height, depth, mipmap, image.format(), min_filter, mag_filter);
	}

	bool Texture3D::load_from_data(unsigned char* data, int width, int height, int depth, bool mipmap, ColorFormat format, TextureMinFilter min_filter, TextureMagFilter mag_filter)
	{
		unsigned int bytesPerRow = width * lly_util::to_color_bpp(format) / 8;
		if (bytesPerRow % 8 == 0)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
		}
		else if (bytesPerRow % 4 == 0)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		}
		else if (bytesPerRow % 2 == 0)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
		}
		else
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		}

		glGenTextures(1, &_tex);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_3D, _tex);
		if (mipmap)
		{
			glTexParameteri(GL_TEXTURE_3D, GL_GENERATE_MIPMAP, GL_TRUE);
		}
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, lly_util::to_min_filter(min_filter));
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, lly_util::to_mag_filter(mag_filter));
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexImage3D(GL_TEXTURE_3D, 0, lly_util::to_color_format(format), (GLsizei)width, (GLsizei)height, (GLsizei)depth, 0, lly_util::to_color_format(format), lly_util::to_color_type(format), data);

		return true;
	}
}
