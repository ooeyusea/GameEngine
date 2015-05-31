#include "PointLightingCommand.h"
#include "Device.h"

namespace lly {
	void PointLightingCommand::draw(Device& device)
	{
		device.setUniformWith1i("u_light_type", 3);
		device.setUniformWith4f("u_light_color", _color.r, _color.g, _color.b, _color.a);
		device.setUniformWith3f("u_light_position", _position.x, _position.y, _position.z);
		device.setUniformWith3f("u_light_attenuation", _constant, _linear, _quadratic);
	}
}
