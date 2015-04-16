#include "Technique.h"
#include "Pass.h"

namespace lly {

	Technique::Technique()
		: _id(0)
	{
	}


	Technique::~Technique()
	{
	}

	Pass* Technique::create_pass()
	{
		Pass* ret = new Pass;
		_passes.push_back(ret);
		return ret;
	}

	void Technique::draw(MeshPart& obj)
	{
		for (auto& pass : _passes)
		{
			pass->draw(obj);
		}
	}

	void Technique::merge_render_option(const RenderOption& option)
	{
		_option.merge(option);

		for (auto& pass : _passes)
		{
			pass->merge_render_option(_option);
		}
	}

	Technique * Technique::clone()
	{
		Technique * ret = new Technique;
		ret->_id = _id;
		ret->_name = _name;
		ret->_option = _option;

		for (auto& pass : _passes)
		{
			ret->_passes.push_back(pass->clone());
		}

		return ret;
	}
}
