#include "Material.h"
#include "Technique.h"
#include "RenderState.h"
#include "../Program.h"

namespace lly {

	Material::Material()
		: _current_tech(nullptr)
	{
	}


	Material::~Material()
	{
		for (auto& tech : _teches)
		{
			delete tech;
		}
		_teches.clear();
	}

	Technique* Material::create_technique(const std::string& name)
	{
		Technique* ret = new Technique;
		_teches.push_back(ret);

		ret->set_name(name);
		unsigned int id = 1;
		for (auto& tech : _teches)
		{
			if (tech->get_id() >= id)
			{
				id = tech->get_id();
			}
		}
		ret->set_id(id);

		return ret;
	}

	void Material::draw(MeshPart& obj)
	{
		if (_current_tech)
			_current_tech->draw(obj);
	}

	void Material::merge_render_option()
	{
		for (auto& tech : _teches)
		{
			tech->merge_render_option(_option);
		}
	}

	void Material::set_current_tech(int id)
	{
		for (auto& tech : _teches)
		{
			if (tech->get_id() == id)
			{
				_current_tech = tech;
				break;
			}
		}
	}

	void Material::set_current_tech(const std::string& name)
	{
		for (auto& tech : _teches)
		{
			if (tech->get_name() == name)
			{
				_current_tech = tech;
				break;
			}
		}
	}
}
