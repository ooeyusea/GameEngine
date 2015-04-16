#include "RenderOption.h"
#include "../../render/Device.h"

namespace lly{

	RenderOption::RenderOption()
	{
	}

	RenderOption::RenderOption(const RenderOption& rhs)
	{
		_state = rhs._state;

		for (auto& itr : rhs._uniforms)
		{
			_uniforms[itr.first] = itr.second;
		}
	}

	RenderOption::RenderOption(RenderOption&& rhs)
	{
		std::swap(_state, rhs._state);
		std::swap(_uniforms, rhs._uniforms);
	}

	RenderOption::~RenderOption()
	{
	}

	RenderOption& RenderOption::operator=(const RenderOption& rhs)
	{
		_state = rhs._state;

		for (auto& itr : rhs._uniforms)
		{
			_uniforms[itr.first] = itr.second;
		}

		return *this;
	}

	RenderOption& RenderOption::operator=(RenderOption&& rhs)
	{
		std::swap(_state, rhs._state);
		std::swap(_uniforms, rhs._uniforms);

		return *this;
	}

	void RenderOption::use(Device& device)
	{
		_state.use(device);

		for (auto& itr : _uniforms)
		{
			itr.second.use(device);
		}
	}

	void RenderOption::add(Uniform& uniform)
	{
		_uniforms[uniform.get_name()] = uniform;
	}

	void RenderOption::set(Uniform& uniform)
	{
		auto itr = _uniforms.find(uniform.get_name());
		if (itr != _uniforms.end())
			itr->second = uniform;
	}

	void RenderOption::set(RenderState& state)
	{
		_state = state;
	}

	RenderOption RenderOption::to_render() const
	{
		RenderOption ret(*this);
		ret.load_inner_uniform();

		return std::move(ret);
	}

	void RenderOption::merge(const RenderOption& rhs)
	{
		_state.merge(rhs._state);

		for (const auto& self_itr : rhs._uniforms)
		{
			_uniforms[self_itr.first].merge(self_itr.second);
		}
	}

	void RenderOption::load_inner_uniform()
	{
		for (auto& itr : _uniforms)
		{
			itr.second.load_inner();
		}
	}
}
