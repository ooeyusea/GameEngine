#include "Image.h"
#include <png.h>
#include <algorithm>

namespace lly {

	const int PNGSIGSIZE = 8;

	struct PngDataSource
	{
		unsigned char* data;
		int size;
		int offset;
	};

	TextureFileType detect_texture_file_type(const std::string& filename)
	{
		if (filename.size() < 5)
			return TextureFileType::UNKOWN;
		std::string suffix = filename.substr(filename.size() - 4);
		std::transform(suffix.begin(), suffix.end(), suffix.begin(), tolower);

		if (suffix == ".png")
			return TextureFileType::PNG;

		return TextureFileType::UNKOWN;
	}

	Image::Image()
		: _format(ColorFormat::DEFAULT)
		, _width(0)
		, _height(0)
	{
	}


	Image::~Image()
	{
	}


	bool Image::load_from_file(const std::string& filename)
	{
		auto data = lly_util::load_from_file(filename);

		TextureFileType type = detect_texture_file_type(filename);
		switch (type)
		{
		case TextureFileType::JPG: return load_from_jpg((unsigned char*)data.data(), data.len());
		case TextureFileType::PNG: return load_from_png((unsigned char*)data.data(), data.len());
		case TextureFileType::TIFF: return load_from_tiff((unsigned char*)data.data(), data.len());
		case TextureFileType::WEBP: return load_from_webp((unsigned char*)data.data(), data.len());
		case TextureFileType::TGA: return load_from_tga((unsigned char*)data.data(), data.len());
		default: return false;
		}

		return true;
	}

	bool Image::change_color_format(ColorFormat format)
	{
		if (format == ColorFormat::DEFAULT)
			return true;



		return true;
	}

	bool Image::load_from_jpg(unsigned char* data, int len)
	{
		return true;
	}

	bool Image::load_from_png(unsigned char* data, int len)
	{
		if (len < PNGSIGSIZE)
			return false;

		png_byte header[PNGSIGSIZE] = { 0 };
		memcpy(header, data, PNGSIGSIZE);
		if (png_sig_cmp(header, 0, PNGSIGSIZE))
			return false;

		png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
		if (png == nullptr)
			return false;

		png_infop info = png_create_info_struct(png);
		if (info == nullptr)
		{
			png_destroy_read_struct(&png, nullptr, nullptr);
			return false;
		}

		PngDataSource data_srouce = { data, len, 0 };
		png_set_read_fn(png, &data_srouce, [](png_structp png_ptr, png_bytep dst, png_size_t length)
		{
			PngDataSource* source = (PngDataSource*)png_get_io_ptr(png_ptr);
			if ((int)(source->offset + length) <= source->size)
			{
				memcpy(dst, source->data + source->offset, length);
				source->offset += length;
			}
			else
			{
				png_error(png_ptr, "pngReaderCallback failed");
			}
		});

		png_read_info(png, info);

		_width = png_get_image_width(png, info);
		_height = png_get_image_height(png, info);
		png_byte bit_depth = png_get_bit_depth(png, info);
		png_uint_32 color_type = png_get_color_type(png, info);

		if (color_type == PNG_COLOR_TYPE_PALETTE)
		{
			png_set_palette_to_rgb(png);
		}

		if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		{
			bit_depth = 8;
			png_set_expand_gray_1_2_4_to_8(png);
		}

		if (png_get_valid(png, info, PNG_INFO_tRNS))
		{
			png_set_tRNS_to_alpha(png);
		}

		if (bit_depth == 16)
		{
			png_set_strip_16(png);
		}

		if (bit_depth < 8)
		{
			png_set_packing(png);
		}

		png_read_update_info(png, info);
		bit_depth = png_get_bit_depth(png, info);
		color_type = png_get_color_type(png, info);

		switch (color_type)
		{
		case PNG_COLOR_TYPE_GRAY:
			_format = ColorFormat::I8;
			break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			_format = ColorFormat::AI88;
			break;
		case PNG_COLOR_TYPE_RGB:
			_format = ColorFormat::RGB888;
			break;
		case PNG_COLOR_TYPE_RGB_ALPHA:
			_format = ColorFormat::RGBA8888;
			break;
		default:
			throw std::logic_error("not support png color format");
			break;
		}

		png_size_t rowbytes;
		png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep)* _height);
		rowbytes = png_get_rowbytes(png, info);

		_data = lly_util::Data(rowbytes * _height * sizeof(unsigned char));

		for (unsigned short i = 0; i < _height; ++i)
		{
			row_pointers[i] = (unsigned char*)_data.data() + i * rowbytes;
		}
		png_read_image(png, row_pointers);
		png_read_end(png, nullptr);

		if (row_pointers != nullptr)
		{
			free(row_pointers);
		}

		png_destroy_read_struct(&png, &info, nullptr);
		return true;
	}

	bool Image::load_from_tiff(unsigned char* data, int len)
	{
		return true;
	}

	bool Image::load_from_webp(unsigned char* data, int len)
	{
		return true;
	}

	bool Image::load_from_tga(unsigned char* data, int len)
	{
		return true;
	}

}

