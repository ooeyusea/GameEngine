#ifndef HEIGHTMAP_H_
#define HEIGHTMAP_H_
#include <vector>

namespace lly{
	class HeightMap
	{
	public:
		HeightMap();
		~HeightMap();

		bool load_from_file(const std::string& filename, float precision);
		bool create(int width, int height, float precision);
		
		float get_height(float x, float y) const;

	private:
		std::vector<float> _datas;
		float _precision;
		int _width;
		int _height;
	};
}

#endif //HEIGHTMAP_H_

