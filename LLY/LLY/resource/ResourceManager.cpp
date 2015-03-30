#include "ResourceManager.h"
#include "Program.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "TextureCube.h"
#include "Mesh.h"
#include "material/Material.h"
#include "material/MaterialLoader.h"
#include "../System.h"
#include "../util/FileUtil.h"
#include <algorithm>
#include "loader/FbxLoader.h"

namespace lly {

	namespace res_helper {

		template <typename T>
		void release(T& t)
		{
			for (auto& itr : t)
			{
				delete itr.second;
			}
			t.clear();
		}
	}

	ResourceManager::ResourceManager()
	{
	}


	ResourceManager::~ResourceManager()
	{
		res_helper::release(_tex2ds);
		res_helper::release(_tex3ds);
		res_helper::release(_texcubes);
		res_helper::release(_programs);
		res_helper::release(_meshs);
	}

	unsigned int ResourceManager::load_texture_2d(const std::string& file)
	{
		unsigned int id = System::instance().get_string_table().get_hash_id(file);

		auto itr = _tex2ds.find(id);
		if (itr == _tex2ds.end())
		{
			Texture2D * texture = new Texture2D;
			texture->set_id(id);
			texture->load_from_file(file);

			_tex2ds[id] = texture;
		}

		return id;
	}

	Texture2D* ResourceManager::get_texture_2d(unsigned int id) const
	{
		auto itr = _tex2ds.find(id);
		if (itr != _tex2ds.end())
		{
			return itr->second;
		}
		return nullptr;
	}

	unsigned int ResourceManager::load_program(const std::string& vertex, const std::string& fragment)
	{
		return load_program(vertex, fragment, "", "", "");
	}

	unsigned int ResourceManager::load_program(const std::string& vertex, const std::string& fragment, const std::string& tess_control, const std::string& tess_evaluation, const std::string& geometry)
	{
		std::string name = "VS:" + vertex + "FS:" + fragment + "TC:" + tess_control + "TE:" + tess_evaluation + "GE:" + geometry;

		unsigned int id = System::instance().get_string_table().get_hash_id(name);
		auto itr = _programs.find(id);
		if (itr == _programs.end())
		{
			lly_util::Data vertex_data = lly_util::load_from_file(vertex);
			lly_util::Data fragment_data = lly_util::load_from_file(fragment);

			Program * program = new Program;
			if (!program->add_vetex_shader(vertex_data.data()))
				throw std::logic_error("compile vertex shader failed:" + vertex);
			if (!program->add_fragment_shader(fragment_data.data()))
				throw std::logic_error("compile fragment shader failed:" + fragment);
			if (tess_control != "")
			{
				lly_util::Data data = lly_util::load_from_file(tess_control);
				if (!program->add_tessellation_control_shader(data.data()))
					throw std::logic_error("compile tessellation control shader failed:" + tess_control);
			}

			if (tess_evaluation != "")
			{
				lly_util::Data data = lly_util::load_from_file(tess_evaluation);
				if (!program->add_tessellation_evaluation_shader(data.data()))
					throw std::logic_error("compile tessellation evaluation shader failed:" + tess_evaluation);
			}

			if (geometry != "")
			{
				lly_util::Data data = lly_util::load_from_file(geometry);
				if (!program->add_geometry_shader(data.data()))
					throw std::logic_error("compile geometry shader failed:" + geometry);
			}

			if (!program->link())
				throw std::logic_error("link program failed:" + vertex);

			_programs[id] = program;
		}

		return id;
	}

	Program* ResourceManager::get_program(unsigned int id) const
	{
		auto itr = _programs.find(id);
		if (itr != _programs.end())
		{
			return itr->second;
		}
		return nullptr;
	}

	unsigned int ResourceManager::load_mesh(const std::string& file)
	{
		unsigned int id = System::instance().get_string_table().get_hash_id(file);
		auto itr = _meshs.find(id);
		if (itr == _meshs.end())
		{
			loader::FbxLoader load;
			if (load.load_file(file))
				_meshs[id] = load.convert_to_resource();
		}

		return id;
	}

	Mesh* ResourceManager::get_mesh(unsigned int id) const
	{
		auto itr = _meshs.find(id);
		if (itr != _meshs.end())
		{
			return itr->second;
		}
		return nullptr;
	}

	unsigned int ResourceManager::load_material(const std::string& file)
	{
		unsigned int id = System::instance().get_string_table().get_hash_id(file);
		auto itr = _materials.find(id);
		if (itr == _materials.end())
		{
			_materials[id] = material_loader::load_material(file);
		}

		return id;
	}

	Material* ResourceManager::get_material(unsigned int id) const
	{
		auto itr = _materials.find(id);
		if (itr != _materials.end())
		{
			return itr->second;
		}
		return nullptr;
	}

	Material* ResourceManager::create_material(const std::string& name)
	{
		unsigned int id = System::instance().get_string_table().get_hash_id(name);
		auto itr = _materials.find(id);
		if (itr == _materials.end())
		{
			_materials[id] = new Material;
		}
		else
			throw std::logic_error("create material duplicate.");

		return _materials[id];
	}

	unsigned int ResourceManager::load_skeleton(const std::string& file, SkeletonData* skeleton)
	{
		unsigned int id = System::instance().get_string_table().get_hash_id(file);
		auto itr = _skeletons.find(id);
		if (itr == _skeletons.end())
			_skeletons[id] = skeleton;
		else
			throw std::logic_error("duplicate skeleton");

		return id;
	}

	SkeletonData* ResourceManager::get_skeleton(unsigned int id) const
	{
		auto itr = _skeletons.find(id);
		if (itr != _skeletons.end())
		{
			return itr->second;
		}
		return nullptr;
	}
}
