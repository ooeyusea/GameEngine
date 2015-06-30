#include "Material.h"
#include "Technique.h"
#include "RenderState.h"
#include "../Program.h"

namespace lly {

	Material::Material()
		: _current_tech(nullptr)
		, _template(false)
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
				id = tech->get_id() + 1;
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

	void Material::draw(IndexBuffer * index)
	{
		if (_current_tech)
			_current_tech->draw(index);
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

	Material * Material::clone()
	{
		Material * ret = new Material;
		ret->_option = _option;
		for (auto& tech : _teches)
		{
			ret->_teches.push_back(tech->clone());
		}
		return ret;
	}
}
