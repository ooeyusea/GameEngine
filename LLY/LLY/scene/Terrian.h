#ifndef TERRIAN_H_
#define TERRIAN_H_
#include "node.h"
#include "Camera.h"

namespace lly{
	class TerrianGrid;
	class VertexBuffer;
	class IndexBuffer;
	class Material;
	class HeightMap;

	class Terrian : public Node, public Camera::Listener
	{
	public:
		Terrian();
		~Terrian();

		void create(float width, float height, int row, int col, HeightMap& height_map);

		void set_camera(Camera * camera) { _camera = camera; }

		virtual void draw() override;
		virtual void on_camera_changed(Camera * camera);

		TerrianGrid * get_grid(int i, int j) const;

	private:
		void recalculate_buffer();

	private:
		VertexBuffer * _vertex;
		IndexBuffer * _index;
		Material * _material;

		float _width;
		float _height;
		int _row;
		int _col;
		std::vector<TerrianGrid*> _grids;

		bool _dirty;
		Camera * _camera;
	};
}

#endif //TERRIAN_H_
