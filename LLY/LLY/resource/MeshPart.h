#ifndef MESHPART_H_
#define MESHPART_H_
#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace lly {
	class IndexBuffer;
	class Mesh;
	class Skeleton;

	class MeshPart
	{
		struct BoneInfo
		{
			std::string name;
			glm::mat4 inverse_bindpose;
		};

	public:
		MeshPart();
		~MeshPart();

		IndexBuffer* get_index_buffer() { return _ib; }
		void set_index_buffer(IndexBuffer* ib) { _ib = ib; }

		void set_material(unsigned int id) { _material = id; }
		unsigned int get_material() const { return _material; }

		void add_bone(const std::string& bone, const glm::mat4& m) { _bones.push_back({ bone, m }); }
		bool has_bone() { return !_bones.empty(); }
		std::vector<glm::mat4> get_matrix_palette(Skeleton * skeleton);

	private:
		IndexBuffer * _ib;
		unsigned int _material;
		std::vector<BoneInfo> _bones;
	};
}

#endif //MESHPART_H_
