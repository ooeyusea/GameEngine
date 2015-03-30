#ifndef IMAGE_H_
#define IMAGE_H_
#include "../util/LLYIncludes.h"
#include "../util/FileUtil.h"

namespace lly {

	class Image
	{
	public:
		Image();
		~Image();

		bool load_from_file(const std::string& filename);

		unsigned char* data() { return (unsigned char*)_data.data(); }
		ColorFormat format() const { return _format;  }
		int width() const { return _width;  }
		int height() const { return _height;  }

		bool change_color_format(ColorFormat format);
		
	private:
		bool load_from_jpg(unsigned char* data, int len);
		bool load_from_png(unsigned char* data, int len);
		bool load_from_tiff(unsigned char* data, int len);
		bool load_from_webp(unsigned char* data, int len);
		bool load_from_tga(unsigned char* data, int len);

	private:
		lly_util::Data _data;

		ColorFormat _format;
		int _width;
		int _height;
	};
}
#endif //IMAGE_H_


