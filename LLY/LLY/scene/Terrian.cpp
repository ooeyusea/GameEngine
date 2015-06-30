#include "Terrian.h"
#include "../resource/material/Material.h"
#include "TerrianGrid.h"
#include "../resource/VertexBuffer.h"
#include "../resource/IndexBuffer.h"
#include "../util/Macros.h"
#include "../resource/HeightMap.h"
#include "../resource/VertexDescription.h"

const int g_lod_size[] = { 2, 4, 8, 16, 32, 64 };

namespace lly {
	Terrian::Terrian()
		: _vertex(nullptr)
		, _index(nullptr)
		, _material(nullptr)
		, _width(0.f)
		, _height(0.f)
		, _row(0)
		, _col(0)
		, _dirty(false)
	{
	}

	Terrian::~Terrian()
	{
	}

	void Terrian::create(float width, float height, int row, int col, HeightMap& height_map)
	{
		_width = width;
		_height = height;
		_row = row;
		_col = col;

		int grid_size = g_lod_size[MAX_LOD - 1];
		int x_step_count = row * grid_size;
		int z_step_count = col * grid_size;

		float step_x = 1.f / (float)x_step_count;
		float step_z = 1.f / (float)z_step_count;

		std::vector<float> vextices;
		for (int i = 0; i < row; ++i)
		{
			for (int j = 0; j < col; ++j)
			{
				for (int u = 0; u <= grid_size; ++u)
				{
					for (int v = 0; v <= grid_size; ++v)
					{
						float x = (float)(i * grid_size + u) / (float)x_step_count;
						float z = (float)(j * grid_size + v) / (float)z_step_count;

						vextices.push_back(x * _width);
						vextices.push_back(height_map.get_height(x, z));
						vextices.push_back(z * _height);
						vextices.push_back(x);
						vextices.push_back(z);
						vextices.push_back((float)u / float(grid_size));
						vextices.push_back((float)v / float(grid_size));

						float h1 = 0.f;
						if (x + step_x > 1.f)
							h1 = height_map.get_height(x + step_x, z) - height_map.get_height(x, z);
						else
							h1 = height_map.get_height(x, z) - height_map.get_height(x - step_x, z);

						float h2 = 0.f;
						if (z - step_z < 0.f)
							h1 = height_map.get_height(x, z - step_z) - height_map.get_height(x, z);
						else
							h1 = height_map.get_height(x, z) - height_map.get_height(x, z + step_z);

						glm::vec3 x_dir(1.f, h1, 0.f);
						glm::vec3 z_dir(0.f, h2, 1.f);
						
						glm::vec3 normal = glm::cross(x_dir, z_dir);
						vextices.push_back(normal.x);
						vextices.push_back(normal.y);
						vextices.push_back(normal.z);
					}
				}

				float x = ((float)i * (float)grid_size + (float)grid_size / 2.f) / (float)x_step_count;
				float z = ((float)j * (float)grid_size + (float)grid_size / 2.f) / (float)z_step_count;

				glm::vec3 check_position(x * _width, height_map.get_height(x, z), z * _height);
				_grids.push_back(new TerrianGrid(*this, i, j, (i * col + j) * (grid_size + 1) * (grid_size + 1), grid_size, check_position));
			}
		}
		_vertex = new VertexBuffer;
		_vertex->load_data((char*)vextices.data(), vextices.size() * sizeof(float), VertexUsage::STATIC_DRAW);

		_index = new IndexBuffer;
		_index->create(row * col * grid_size * grid_size * 2);

		VertexDescription desc(DrawType::TRIANGLES, 0);
		desc.add(lly::VertexAttribPosition::POSITION, 3, lly::DataType::FLOAT, false, 10 * sizeof(float), 0);
		desc.add(lly::VertexAttribPosition::TEXCOOD0, 2, lly::DataType::FLOAT, false, 10 * sizeof(float), 3 * sizeof(float));
		desc.add(lly::VertexAttribPosition::TEXCOOD1, 2, lly::DataType::FLOAT, false, 10 * sizeof(float), 5 * sizeof(float));
		desc.add(lly::VertexAttribPosition::NORMAL, 3, lly::DataType::FLOAT, false, 10 * sizeof(float), 7 * sizeof(float));
		_index->gen(_vertex, desc);
	}

	void Terrian::draw()
	{
		if (_dirty)
			recalculate_buffer();

		_material->draw(_index);
	}

	void Terrian::on_camera_changed(Camera * camera)
	{
		_dirty = true;
	}

	TerrianGrid * Terrian::get_grid(int i, int j) const
	{
		if (i < 0 || i >= _row || j < 0 || j >= _col)
			return nullptr;

		return _grids[i * _col + j];
	}

	void Terrian::recalculate_buffer()
	{
		for (auto grid : _grids)
			grid->recalculate_lod(_transform, _camera->get_world_position());

		std::vector<int> indices;
		for (auto grid : _grids)
			grid->recalculate(indices);
		_index->update((const char*)indices.data(), indices.size() * sizeof(int), indices.size());
	}
}
