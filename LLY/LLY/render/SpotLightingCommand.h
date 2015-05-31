#ifndef SPOTLIGHTCOMMAND_H_
#define SPOTLIGHTCOMMAND_H_
#include <glm/glm.hpp>
#include "LightingCommand.h"

namespace lly {
	class Device;

	class SpotLightingCommand : public LightingCommand
	{
	public:
		SpotLightingCommand(const glm::vec4& color, 
							const glm::vec3& position, 
							const glm::vec3& direction, 
							float constant, 
							float linear, 
							float quadratic,
							float exponent,
							float cutoff)
			: _color(color)
			, _position(position)
			, _direction(direction)
			, _constant(constant)
			, _linear(linear)
			, _quadratic(quadratic)
			, _exponent(exponent)
			, _cutoff(cutoff)
		{
		}
		virtual ~SpotLightingCommand() {}

		virtual void draw(Device& device);

	private:
		glm::vec4 _color;
		glm::vec3 _position;
		glm::vec3 _direction;
		float _constant;
		float _linear;
		float _quadratic;
		float _exponent;
		float _cutoff;
	};
}

#endif //SPOTLIGHTCOMMAND_H_
