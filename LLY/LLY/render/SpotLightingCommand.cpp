#include "SpotLightingCommand.h"
#include "Device.h"

namespace lly {
	void SpotLightingCommand::draw(Device& device)
	{
		device.setUniformWith1i("u_light_type", 4);
		device.setUniformWith4f("u_light_color", _color.r, _color.g, _color.b, _color.a);
		device.setUniformWith3f("u_light_position", _position.x, _position.y, _position.z);
		device.setUniformWith3f("u_light_direction", _direction.x, _direction.y, _direction.z);
		device.setUniformWith3f("u_light_attenuation", _constant, _linear, _quadratic);
		device.setUniformWith2f("u_light_cutoff", _exponent, _cutoff);
	}
}
