#ifndef POINTLIGHTCOMMAND_H_
#define POINTLIGHTCOMMAND_H_
#include <glm/glm.hpp>
#include "LightingCommand.h"

namespace lly {
	class Device;

	class PointLightingCommand : public LightingCommand
	{
	public:
		PointLightingCommand(const glm::vec4& color, const glm::vec3& position, float constant, float linear, float quadratic)
			: _color(color), _position(position), _constant(constant), _linear(linear), _quadratic(quadratic)
		{
		}
		virtual ~PointLightingCommand() {}

		virtual void draw(Device& device);

	private:
		glm::vec4 _color;
		glm::vec3 _position;
		float _constant;
		float _linear;
		float _quadratic;
	};
}

#endif //POINTLIGHTCOMMAND_H_
