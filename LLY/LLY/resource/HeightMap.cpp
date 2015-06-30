#include "HeightMap.h"
#include "Image.h"

namespace lly {
	HeightMap::HeightMap()
		: _precision(0.f)
		, _width(0)
		, _height(0)
	{
	}

	HeightMap::~HeightMap()
	{
	}

	bool HeightMap::load_from_file(const std::string& filename, float precision)
	{
		Image image;
		if (!image.load_from_file(filename))
			return false;

		_width = image.get_width();
		_height = image.get_height();
		_precision = precision;

		for (int j = 0; j < _height; ++j)
		{
			for (int i = 0; i < _width; ++i)
			{
				float r, g, b;
				image.get_rgb(i, j, r, g, b);
				_datas.push_back(r);
			}
		}
	}

	bool HeightMap::create(int width, int height, float precision)
	{
		_datas.resize(width * height, 0.f);
		_width = width;
		_height = height;
		_precision = precision;

		return true;
	}

	float HeightMap::get_height(float x, float y) const
	{
		int x1 = (int)(x * _width);
		int x2 = x1 + 1;
		float x_left = x * _width - x1;

		int y1 = (int)(y * _height);
		int y2 = y1 + 1;
		float y_left = y * _height - x1;

		return _datas[y1 * _width + x1] * (1 - x_left) * (1 - y_left)
			+ _datas[(y1 + 1) * _width + x1] * (1 - x_left) * y_left
			+ _datas[(y1 + 1) * _width + x1 + 1] * x_left * y_left
			+ _datas[y1 * _width + x1 + 1] * x_left * (1 - y_left);
	}
}
