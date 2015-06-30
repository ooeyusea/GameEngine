#include "TerrianGrid.h"
#include "../System.h"
#include "../resource/Mesh.h"
#include "../resource/VertexBuffer.h"
#include "Terrian.h"
#include <math.h>

namespace lly {
	TerrianGrid::TerrianGrid(Terrian& parent, int row, int col, int start, int grid_size, const glm::vec3& position)
		: _parent(parent)
		, _row(row)
		, _col(col)
		, _start(start)
		, _grid_size(grid_size)
		, _position(position)
		, _lod(0)
	{
	}

	TerrianGrid::~TerrianGrid()
	{
	}

	void TerrianGrid::recalculate_lod(const glm::mat4& tranform, const glm::vec3& eye)
	{
		auto tmp = tranform * glm::vec4(_position, 1.f);
		glm::vec3 world_position(tmp.x, tmp.y, tmp.z);

		float distance = glm::distance(world_position, eye);

		if (distance <= 15.f)
			_lod = 2;
		else if (distance <= 50.f)
			_lod = 1;
		else
			_lod = 0;
	}

	void TerrianGrid::recalculate(std::vector<int>& indices)
	{
		auto left = _parent.get_grid(_row, _col - 1);
		auto right = _parent.get_grid(_row, _col + 1);
		auto top = _parent.get_grid(_row - 1, _col);
		auto bottom = _parent.get_grid(_row + 1, _col);

		int cell = (int)pow(2, _lod);
		int step = _grid_size / cell;

		for (int i = 0; i < cell; ++i)
		{
			for (int j = 0; j < cell; ++j)
			{
				int left_lod = (j == 0 && left ? left->get_lod() : _lod);
				int right_lod = (j == 0 && left ? left->get_lod() : _lod);
				int top_lod = (j == 0 && left ? left->get_lod() : _lod);
				int bottom_lod = (j == 0 && left ? left->get_lod() : _lod);

				int p1 = i * step * (_grid_size + 1) + j * step;
				int p2 = i * step * (_grid_size + 1) + (j + 1) * step;
				int p3 = (i + 1) * step * (_grid_size + 1) + (j + 1) * step;
				int p4 = (i + 1) * step * (_grid_size + 1) + j * step;
				int middle = (i * step + step / 2) * (_grid_size + 1) + (j * step + step / 2);

				recalculate_one(indices, p1, p2, p3, p4, middle, left_lod, right_lod, top_lod, bottom_lod);
			}
		}
	}

	void TerrianGrid::recalculate_lod_0(std::vector<int>& indices)
	{
		auto left = _parent.get_grid(_row, _col - 1);
		auto right = _parent.get_grid(_row, _col + 1);
		auto top = _parent.get_grid(_row - 1, _col);
		auto bottom = _parent.get_grid(_row + 1, _col);
		

	}

	void TerrianGrid::recalculate_lod_1(std::vector<int>& indices)
	{
		auto left = _parent.get_grid(_row, _col - 1);
		auto right = _parent.get_grid(_row, _col + 1);
		auto top = _parent.get_grid(_row - 1, _col);
		auto bottom = _parent.get_grid(_row + 1, _col);

		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{

			}
		}
	}

	void TerrianGrid::recalculate_lod_2(std::vector<int>& indices)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{

			}
		}
	}

	void TerrianGrid::recalculate_one(std::vector<int>& indices, int p1, int p2, int p3, int p4, int middle, int left, int right, int top, int bottom)
	{
		// top
		{
			int step = p2 - p1;
			if (top > _lod)
				step = step / (int)pow(2, top - _lod);
		
			for (int i = p1; i < p2; i += step)
			{
				indices.push_back(_start + middle);
				indices.push_back(_start + i);
				indices.push_back(_start + i + step);
			}
		}

		// right
		{
			int step = p3 - p2;
			if (right > _lod)
				step = step / (int)pow(2, right - _lod);

			for (int i = p2; i < p3; i += step)
			{
				indices.push_back(_start + middle);
				indices.push_back(_start + i);
				indices.push_back(_start + i + step);
			}
		}
		// bottom
		{
			int step = p3 - p4;
			if (bottom > _lod)
				step = step / (int)pow(2, bottom - _lod);

			for (int i = p3; i < p4; i += step)
			{
				indices.push_back(_start + middle);
				indices.push_back(_start + i + step);
				indices.push_back(_start + i);
			}
		}

		// left
		{
			int step = p4 - p1;
			if (left > _lod)
				step = step / (int)pow(2, left - _lod);

			for (int i = p1; i < p4; i += step)
			{
				indices.push_back(_start + middle);
				indices.push_back(_start + i + step);
				indices.push_back(_start + i);
			}
		}
	}
}
