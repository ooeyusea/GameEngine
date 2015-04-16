#ifndef FBXLOADER_H_
#define FBXLOADER_H_
#include <string>
#include <fbxsdk.h>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include <algorithm>

namespace lly {
	class Mesh;
	class VertexDescription;
	class SkeletonData;
}

namespace loader {

	enum Attribute
	{
		POSITION = 0x01,
		NORMAL = 0x02,
		COLOR = 0x04,
		TANGENT = 0x08,
		TEXCOOD0 = 0x20,
		TEXCOOD1 = 0x40,
		TEXCOOD2 = 0x80,
		TEXCOOD3 = 0x100,
		TEXCOOD4 = 0x200,
		TEXCOOD5 = 0x400,
		TEXCOOD6 = 0x800,
		TEXCOOD7 = 0x1000,
		WEIGHT0 = 0x2000,
		WEIGHT1 = 0x4000,
		WEIGHT2 = 0x8000,
		WEIGHT3 = 0x10000,
	};

	struct MeshInfo
	{
		std::unordered_map<int, std::vector<std::tuple<int, float>>> point_blend_weights;
		std::unordered_map<int, int> triangle_part;
		std::unordered_map<int, std::vector<std::vector<int>>> part_bones;
		std::unordered_map<int, int> triangle_part_bones_map;
	};

	struct MeshPart
	{
		glm::mat4 pre_matrix;
		std::string material;
		std::vector<int> indices;
		std::vector<std::tuple<std::string, glm::mat4>> bones;
	};

	template <typename T>
	struct OptionalValue
	{
		bool valid;
		T value;

		OptionalValue() : valid(false), value((T)0) {}

		inline void unset() {
			valid = false;
		}

		inline void set(const T& v) {
			valid = true;
			value = v;
		}

		inline OptionalValue& operator=(const T& v)
		{
			set(v);

			return *this;
		}
	};

	struct Material
	{
		struct Texture
		{
			enum class Usage 
			{
				Unknown,
				Diffuse,
				Emissive,
				Specular,
				Shininess,
				Normal
			};

			std::string id;
			std::string path;
			float uv_translation[2];
			float uv_scale[2];
			Usage usage;

			Texture() : usage(Usage::Unknown)
			{
				uv_translation[0] = uv_translation[1] = 0.f;
				uv_scale[0] = uv_scale[1] = 1.f;
			}
		};

		std::string name;
		OptionalValue<glm::vec4> ambient;
		OptionalValue<glm::vec4> diffuse;
		OptionalValue<glm::vec4> emissive;
		OptionalValue<glm::vec4> specular;
		OptionalValue<float> shininess;
		OptionalValue<float> opacity;

		std::vector<Texture> textures;
	};

	struct AnimInfo 
	{
		float start;
		float stop;
		float framerate;
		bool translate;
		bool rotate;
		bool scale;

		AnimInfo() : start(FLT_MAX), stop(-1.f), framerate(0.f), translate(false), rotate(false), scale(false) {}

		inline AnimInfo& operator+=(const AnimInfo& rhs) 
		{
			start = std::min(rhs.start, start);
			stop = std::max(rhs.stop, stop);
			framerate = std::max(rhs.framerate, framerate);
			translate = translate || rhs.translate;
			rotate = rotate || rhs.rotate;
			scale = scale || rhs.scale;
			return *this;
		}
	};

	class FbxLoader
	{
	public:
		FbxLoader();
		~FbxLoader();

		bool load_file(const std::string& file);

		lly::Mesh * convert_to_resource();

	private:
		void setup(FbxNode * root);
		void setup_node(FbxNode * node);

		void travel(FbxNode * root);
		void read_mesh(FbxNode * node);

		void cal_vertex_size();
		void read_vertex(FbxNode * node, float * vertex, int poly_index, int index, int v, int poly_inside_index);
		int add_vertex(MeshPart& part, float * vertex);

		void construct_bone(FbxNode * root);
		void construct_skeleton(FbxNode * root);

		void read_animation(FbxScene * scene);

		void read_material(FbxScene * scene);
		void add_textures(std::vector<Material::Texture>& textures, const FbxProperty& prop, const Material::Texture::Usage& usage);
		std::unordered_map<std::string, unsigned int> convert_materials();

		std::string _file;
		int _vertex_format;
		std::unordered_map<FbxMesh*, MeshInfo> _mesh_infos;
		std::unordered_map<FbxSurfaceMaterial*, Material> _materials;

		void descript_vertex(lly::VertexDescription& desc);

		int _vertex_size;
		int _vertex_count;
		std::vector<float> _vertices;
		std::vector<MeshPart> _parts;
		lly::SkeletonData * _skeleton;
	};
}

#endif //FBXLOADER_H_

