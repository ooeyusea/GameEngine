#include "Texture2D.h"
#include "../util/FileUtil.h"
#include <png.h>
#include "Image.h"
#include "../util/FuncUnitl.h"

namespace lly {

	Texture2D::Texture2D()
		: _tex(0)
		, _format(ColorFormat::DEFAULT)
		, _width(0)
		, _height(0)
		, _warp_s(TextureWrap::CLAMP)
		, _warp_t(TextureWrap::CLAMP)
		, _min_filter(TextureMinFilter::NEAREST)
		, _mag_filter(TextureMagFilter::NEAREST)
		, _mipmap(false)
	{
	}


	Texture2D::~Texture2D()
	{
		if (_tex > 0)
		{
			glDeleteTextures(1, &_tex);
			_tex = 0;
		}
	}

	bool Texture2D::load_from_file(const std::string& filename)
	{
		return load_from_file(filename, true, ColorFormat::DEFAULT, TextureWrap::CLAMP, TextureWrap::CLAMP, TextureMinFilter::LINEAR, TextureMagFilter::LINEAR);
	}

	bool Texture2D::load_from_file(const std::string& filename, bool mipmap, ColorFormat format, TextureWrap s, TextureWrap t, TextureMinFilter min_filter, TextureMagFilter mag_filter)
	{
		_name = filename;
		Image image;
		if (!image.load_from_file(filename))
			return false;

		if (!image.change_color_format(format))
			return false;

		return load_from_data(image.data(), image.width(), image.height(), mipmap, image.format(), s, t, min_filter, mag_filter);
	}

	bool Texture2D::load_from_data(unsigned char* data, int width, int height, bool mipmap, ColorFormat format, TextureWrap s, TextureWrap t, TextureMinFilter min_filter, TextureMagFilter mag_filter)
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
		glBindTexture(GL_TEXTURE_2D, _tex);
		if (mipmap)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, lly_util::to_min_filter(min_filter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, lly_util::to_mag_filter(mag_filter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, lly_util::to_warp(s));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, lly_util::to_warp(t));
		glTexImage2D(GL_TEXTURE_2D, 0, lly_util::to_color_format(format), (GLsizei)width, (GLsizei)height, 0, lly_util::to_color_format(format), lly_util::to_color_type(format), data);

		_format = format;
		_width = width;
		_height = height;
		_warp_s = s;
		_warp_t = t;
		_min_filter = min_filter;
		_mag_filter = mag_filter;

		return true;
	}

	bool Texture2D::create(int width, int height, bool mipmap, ColorFormat format, TextureWrap s, TextureWrap t, TextureMinFilter min_filter, TextureMagFilter mag_filter)
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
		glBindTexture(GL_TEXTURE_2D, _tex);
		if (mipmap)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, lly_util::to_min_filter(min_filter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, lly_util::to_mag_filter(mag_filter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, lly_util::to_warp(s));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, lly_util::to_warp(t));
		glTexImage2D(GL_TEXTURE_2D, 0, lly_util::to_color_format(format), (GLsizei)width, (GLsizei)height, 0, lly_util::to_color_format(format), lly_util::to_color_type(format), nullptr);
		
		_format = format;
		_width = width;
		_height = height;
		_warp_s = s;
		_warp_t = t;
		_min_filter = min_filter;
		_mag_filter = mag_filter;

		return true;
	}

	void Texture2D::attach_to_render_target(int index)
	{
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, _tex, 0);
	}

	void Texture2D::set_warp(TextureWrap s, TextureWrap t)
	{
		if (_warp_s != s)
		{
			_warp_s = s;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, lly_util::to_warp(s));
		}

		if (_warp_t != t)
		{
			_warp_t = t;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, lly_util::to_warp(t));
		}
	}

	void Texture2D::set_filter(TextureMinFilter min, TextureMagFilter mag)
	{
		if (_min_filter != min)
		{
			_min_filter = min;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, lly_util::to_min_filter(min));
		}

		if (_mag_filter != mag)
		{
			_mag_filter = mag;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, lly_util::to_mag_filter(mag));
		}
	}

	void Texture2D::gen_mipmap()
	{
		if (!_mipmap)
		{
			_mipmap = true;
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

}
