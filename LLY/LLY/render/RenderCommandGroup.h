#ifndef RENDERCOMMANDGROUP_H_
#define RENDERCOMMANDGROUP_H_
#include <vector>

namespace lly {
	class RenderTarget;
	class RenderCommand;
	class Device;

	class RenderCommandGroup
	{
	public:
		RenderCommandGroup();
		~RenderCommandGroup();

		bool operator<(const RenderCommandGroup& rhs) const;

		void draw(Device& device);

		void set_target(RenderTarget* target) { _target = target; }
		RenderTarget * get_target() const { return _target; }

		void set_clear(int clear) { _clear = clear;  }

		void add(RenderCommand* command) { _commands.push_back(command); }

		void sort() {}

	private:
		RenderTarget * _target;
		int _clear;
		std::vector<RenderCommand*> _commands;
	};

}

#endif //RENDERCOMMANDGROUP_H_


