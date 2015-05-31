#ifndef AMBIENTCOMMAND_H_
#define AMBIENTCOMMAND_H_
#include <glm/glm.hpp>
#include "LightingCommand.h"

namespace lly {
	class Device;

	class AmbientCommand : public LightingCommand
	{
	public:
		AmbientCommand(const glm::vec4& color) : _ambient(color) {}
		virtual ~AmbientCommand() {}

		virtual void draw(Device& device);

	private:
		glm::vec4 _ambient;
	};
}

#endif //AMBIENTCOMMAND_H_
