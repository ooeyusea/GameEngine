#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_
#include <unordered_map>

namespace lly {
	class Texture2D;
	class Texture3D;
	class TextureCube;
	class Mesh;
	class Program;
	class Material;
	class SkeletonData;

	class ResourceManager
	{
		enum class ResourceType
		{
			NONE,
			PROGRAM,
			TEXTURE2D,
			TEXTURE3D,
			TEXTURECUBE,
			VERTEXBUFFER,
			MATERIAL,
		};

	public:
		ResourceManager();
		~ResourceManager();

		unsigned int load_texture_2d(const std::string& file);
		Texture2D* get_texture_2d(unsigned int id) const;

		unsigned int load_program(const std::string& vertex, const std::string& fragment);
		unsigned int load_program(const std::string& vertex, const std::string& fragment, const std::string& tess_control, const std::string& tess_evaluation, const std::string& geometry);
		Program* get_program(unsigned int id) const;

		unsigned int load_mesh(const std::string& file);
		Mesh* get_mesh(unsigned int id) const;

		unsigned int load_material(const std::string& file);
		Material* get_material(unsigned int id) const;
		Material* create_material(const std::string& name);

		unsigned int load_skeleton(const std::string& file, SkeletonData* skeleton);
		SkeletonData* get_skeleton(unsigned int id) const;

	private:
		std::unordered_map<unsigned int, Texture2D*> _tex2ds;
		std::unordered_map<unsigned int, Texture3D*> _tex3ds;
		std::unordered_map<unsigned int, TextureCube*> _texcubes;
		std::unordered_map<unsigned int, Program*> _programs;
		std::unordered_map<unsigned int, Mesh*> _meshs;
		std::unordered_map<unsigned int, Material*> _materials;
		std::unordered_map<unsigned int, SkeletonData*> _skeletons;
	};

}

#endif //RESOURCEMANAGER_H_


