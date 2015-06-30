#ifndef TERRIANGRID_H__
#define TERRIANGRID_H__
#include "Camera.h"
#include "node.h"
#include <unordered_map>
#include <vector>

namespace lly {
	class Terrian;
	class Camera;
	class TerrianGrid
	{
	public:
		TerrianGrid(Terrian& parent, int row, int col, int start, int grid_size, const glm::vec3& position);
		~TerrianGrid();

		void recalculate_lod(const glm::mat4& tranform, const glm::vec3& eye);
		void recalculate(std::vector<int>& indices);

		int get_lod() const { return _lod; }

	private:
		void recalculate_lod_0(std::vector<int>& indices);
		void recalculate_lod_1(std::vector<int>& indices);
		void recalculate_lod_2(std::vector<int>& indices);
		void recalculate_one(std::vector<int>& indices, int p1, int p2, int p3, int p4, int middle, int left, int right, int top, int bottom);

	private:
		Terrian& _parent;
		int _row;
		int _col;
		int _start;
		int _grid_size;
		glm::vec3 _position;

		int _lod;
	};
}

#endif //TERRIANGRID_H__
