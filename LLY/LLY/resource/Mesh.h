#ifndef MESH_H_
#define MESH_H_
#include <vector>
#include "Resource.h"
#include <string>
#include <glm/glm.hpp>

namespace lly {
	class MeshPart;
	class VertexBuffer;
	class Skeleton;

	class Mesh : public Resource
	{
	public:
		Mesh();
		~Mesh();

		VertexBuffer* get_vertex_buffer() { return _vb; }
		void set_vertex_buffer(VertexBuffer* vb) { _vb = vb; }

		MeshPart * create_part();
		void add_part(MeshPart * part);
		MeshPart * get_part(unsigned int index) const;
		int get_part_count() const { return _parts.size(); }

		void set_skeleton(unsigned int skeleton) { _skeleton = skeleton; }
		unsigned int get_skeleton() const { return _skeleton; }
		bool has_skeleton() const { return _skeleton != 0; }

		void draw();
		void draw(Skeleton * skeleton);

	private:
		VertexBuffer* _vb;
		std::vector<MeshPart*> _parts;
		unsigned int _skeleton;
	};

	namespace mesh_loader {
		Mesh * load_mesh(const std::string& file);
		Mesh * load_fbx(const std::string& file);
	}
}

#endif //MESH_H_
