#ifndef RENDERCOMMAND_H_
#define RENDERCOMMAND_H_
#include <unordered_map>
#include "../resource/material/RenderOption.h"

namespace lly {
	class Device;
	class Program;
	class IndexBuffer;
	class Uniform;

	class RenderCommand
	{
	public:
		RenderCommand();
		~RenderCommand();

		bool operator<(const RenderCommand& rhs) const;

		void set_batch(int batch) { _batch = batch;  }

		void set_program(Program* program) { _program = program; }
		void set_index_buffer(IndexBuffer* index) { _index = index; }
		void set_render_option(const RenderOption& option) { _option = option; }

		virtual void draw(Device& device);

	private:
		int _batch;
		Program* _program;
		IndexBuffer* _index;
		RenderOption _option;
	};
}

#endif //RENDERCOMMAND_H_


