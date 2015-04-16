#ifndef RENDEROPTION_H_
#define RENDEROPTION_H_
#include "Uniform.h"
#include "RenderState.h"
#include <unordered_map>

namespace lly {
	class Device;

	class RenderOption
	{
	public:
		RenderOption();
		RenderOption(const RenderOption&);
		RenderOption(RenderOption&&);
		~RenderOption();

		RenderOption& operator=(const RenderOption&);
		RenderOption& operator=(RenderOption&&);

		void use(Device& device);
		void add(Uniform& uniform);
		void set(Uniform& uniform);
		void set(RenderState& state);

		RenderOption to_render() const;

		void merge(const RenderOption& rhs);
	private:
		void load_inner_uniform();

		std::unordered_map<std::string, Uniform> _uniforms;
		RenderState _state;
	};

}

#endif //RENDEROPTION_H_
