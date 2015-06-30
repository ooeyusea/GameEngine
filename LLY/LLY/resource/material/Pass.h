#ifndef PASS_H_
#define PASS_H_
#include "RenderOption.h"

namespace lly {
	class Device;
	class MeshPart;
	class IndexBuffer;

	class Pass
	{
	public:
		Pass();
		~Pass();

		void set_id(int id) { _id = id; }
		int get_id() const { return _id; }

		void set_program(unsigned int program) { _program = program; }
		void add_uniform(Uniform& uniform) { _option.add(uniform); }
		void set_render_state(RenderState& state) { _option.set(state); }

		void draw(MeshPart& obj);
		void draw(IndexBuffer * index);

		void merge_render_option(const RenderOption& option);

		Pass * clone();
	private:
		int _id;
		unsigned int _program;
		RenderOption _option;
	};

}

#endif //PASS_H_
