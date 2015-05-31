#ifndef LIGHTINGCOMMAND_H_
#define LIGHTINGCOMMAND_H_

namespace lly {
	class Device;

	class LightingCommand
	{
	public:
		LightingCommand() {}
		virtual ~LightingCommand() {}

		virtual void draw(Device& device) = 0;
	};
}

#endif //LIGHTINGCOMMAND_H_
