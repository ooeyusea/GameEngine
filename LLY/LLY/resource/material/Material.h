#ifndef MATERIAL_H_
#define MATERIAL_H_
#include <vector>
#include "RenderOption.h"
#include "../Resource.h"

namespace lly {
	class Technique;
	class Device;
	class MeshPart;
	class IndexBuffer;

	class Material : public Resource
	{
	public:
		Material();
		~Material();

		void set_name(const std::string& name) { _name = name; }
		const std::string& get_name() const { return _name; }

		void add_technique(Technique* tech) { _teches.push_back(tech); }
		Technique* create_technique(const std::string& name);

		void add_uniform(Uniform& uniform) { _option.add(uniform); }
		void set_render_state(RenderState& state) { _option.set(state); }
		void set_uniform(Uniform& uniform) { _option.set(uniform); }

		void draw(MeshPart& obj);
		void draw(IndexBuffer * index);

		void merge_render_option();

		void set_current_tech(int id);
		void set_current_tech(const std::string& name);

		void set_template(bool val) { _template = val; }
		bool is_template() const { return _template; }

		Material * clone();

	private:
		std::string _name;

		RenderOption _option;

		Technique* _current_tech;
		std::vector<Technique*> _teches;
		bool _template;
	};
}

#endif //MATERIAL_H_
