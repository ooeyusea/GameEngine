#ifndef TECHNIQUE_H_
#define TECHNIQUE_H_
#include "RenderOption.h"
#include <string>

namespace lly {
	class Pass;
	class Device;
	class MeshPart;

	class Technique
	{
	public:
		Technique();
		~Technique();

		void set_id(int id) { _id = id; }
		int get_id() const { return _id; }

		void set_name(const std::string& name) { _name = name; }
		const std::string& get_name() const { return _name; }

		void add_pass(Pass* pass) { _passes.push_back(pass); }
		Pass* create_pass();

		void add_uniform(Uniform& uniform) { _option.add(uniform); }
		void set_render_state(RenderState& state) { _option.set(state); }

		void draw(MeshPart& obj);

		void merge_render_option(const RenderOption& option);
	private:
		int _id;
		std::string _name;

		RenderOption _option;
		std::vector<Pass*> _passes;
	};

}

#endif //TECHNIQUE_H_

