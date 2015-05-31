#ifndef DIRECTLIGHTCOMMAND_H_
#define DIRECTLIGHTCOMMAND_H_
#include <glm/glm.hpp>
#include "LightingCommand.h"

namespace lly {
	class Device;

	class DirectLightCommand : public LightingCommand
	{
	public:
		DirectLightCommand(const glm::vec4& color, const glm::vec3& direction) : _color(color), _direction(direction){}
		virtual ~DirectLightCommand() {}

		virtual void draw(Device& device);

	private:
		glm::vec4 _color;
		glm::vec3 _direction;
	};
}

#endif //DIRECTLIGHTCOMMAND_H_
